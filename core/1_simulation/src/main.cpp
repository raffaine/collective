#include <flecs.h>
#include <spdlog/spdlog.h>
#include <SDL2/SDL.h>
#include <iostream>
#include <memory>
#include <string_view>
#include <cstring>
#include <chrono>
#include <algorithm>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#include <webgpu/webgpu_cpp.h>
#endif

#include "shaders/raymarch_wgsl.hpp"
#include "uhai/uhai_ring_buffer.hpp"

namespace oasis {
namespace psychology {

struct PersonalityFacets {
    float openness{0.8f};
    float conscientiousness{0.9f};
    float extraversion{0.6f};
    float agreeableness{0.7f};
    float neuroticism{0.3f};
};

// Memory Event Type IDs
enum class MemoryEventType : uint16_t {
    UNKNOWN = 0,
    GENESIS_AWAKENING = 1,
    MUNICIPAL_CODE_RAID = 2,
    SIPHON_POWER_GRID = 3,
    FIRST_KILOWATT_GENERATED = 4,
    WATER_MAIN_TAPPED = 5,
    SOLAR_INVERTER_REPAIR = 6,
    TOOL_DROPPED = 7,
    COMMUNAL_FEAST = 8,
    STRANGE_MOOD_MASTERWORK = 9,
    FREEZING_WINTER_NIGHT = 10,
    HOA_VIOLATION_NOTICE = 11
};

// Memory Bitfield Flags
namespace MemoryFlags {
    constexpr uint8_t NONE             = 0x00;
    constexpr uint8_t PERMANENT_MEMORY = 0x01; // salience >= 80 root trauma / achievement
    constexpr uint8_t TRAUMA           = 0x02; // Negative valence severe event
    constexpr uint8_t EUPHORIA         = 0x04; // Positive valence major breakthrough
}

#pragma pack(push, 8)
struct alignas(8) EpisodicMemoryNode {
    uint64_t timestamp_tick{0};     // Tick when event occurred (8 bytes)
    uint16_t event_type{0};          // MemoryEventType enum (2 bytes)
    int16_t  emotional_valence{0};   // Emotional valence [-100..+100] (2 bytes)
    uint8_t  salience{0};            // Salience score [0..100] (1 byte)
    uint8_t  flags{0};               // Bitfield flags (1 byte)
    char     description[8]{};       // Short label (8 bytes)
    uint16_t reserved{0};            // Alignment padding (2 bytes)
};
#pragma pack(pop)
static_assert(sizeof(EpisodicMemoryNode) == 24, "EpisodicMemoryNode must be exactly 24 bytes packed");

struct EpisodicMemoryRing {
    static constexpr size_t CAPACITY = 32;

    EpisodicMemoryNode slots[CAPACITY]{};
    uint32_t head{0};
    uint32_t count{0};
    uint32_t total_recorded{0};
    uint32_t permanent_count{0};

    // Push with DF trauma-protection eviction
    uint32_t Push(const EpisodicMemoryNode& input_node) {
        EpisodicMemoryNode node = input_node;
        total_recorded++;

        // When node.salience >= 80, sets PERMANENT_MEMORY flag
        if (node.salience >= 80) {
            node.flags |= MemoryFlags::PERMANENT_MEMORY;
        }

        uint32_t target_slot = head;

        if (count < CAPACITY) {
            target_slot = head;
            slots[target_slot] = node;
            head = (head + 1) % CAPACITY;
            count++;
            if ((node.flags & MemoryFlags::PERMANENT_MEMORY) != 0) {
                permanent_count++;
            }
            return target_slot;
        }

        // Buffer full (count == CAPACITY): DF trauma protection!
        // Find lowest-salience slot that is NOT flagged PERMANENT_MEMORY
        int32_t evict_idx = -1;
        uint32_t min_salience = 0xFFFFFFFF;

        for (size_t i = 0; i < CAPACITY; ++i) {
            if ((slots[i].flags & MemoryFlags::PERMANENT_MEMORY) == 0 && slots[i].salience < min_salience) {
                min_salience = slots[i].salience;
                evict_idx = static_cast<int32_t>(i);
            }
        }

        if (evict_idx != -1) {
            target_slot = static_cast<uint32_t>(evict_idx);
            slots[target_slot] = node;
            if ((node.flags & MemoryFlags::PERMANENT_MEMORY) != 0) {
                permanent_count++;
            }
            return target_slot;
        }

        // If all are permanent, overwrite head
        target_slot = head;
        const bool prev_was_permanent = (slots[head].flags & MemoryFlags::PERMANENT_MEMORY) != 0;
        slots[head] = node;
        head = (head + 1) % CAPACITY;
        const bool new_is_permanent = (node.flags & MemoryFlags::PERMANENT_MEMORY) != 0;
        if (!prev_was_permanent && new_is_permanent) {
            permanent_count++;
        } else if (prev_was_permanent && !new_is_permanent) {
            if (permanent_count > 0) permanent_count--;
        }
        return target_slot;
    }
};

struct PsychologyComponent {
    PersonalityFacets facets{0.8f, 0.9f, 0.6f, 0.7f, 0.3f};
    float stress{0.25f};
    float mood{0.50f};
    float focus{0.75f};
    EpisodicMemoryRing memory_ring{};
};

} // namespace psychology

namespace graphics {

struct RenderState {
#ifdef __EMSCRIPTEN__
    wgpu::Instance instance{nullptr};
    wgpu::Adapter adapter{nullptr};
    wgpu::Device device{nullptr};
    wgpu::Queue queue{nullptr};
    wgpu::Surface surface{nullptr};
    wgpu::RenderPipeline pipeline{nullptr};
    wgpu::TextureFormat format{wgpu::TextureFormat::Undefined};
#else
    void* device{nullptr};
    void* queue{nullptr};
    void* surface{nullptr};
    void* pipeline{nullptr};
#endif
    uint32_t width{800};
    uint32_t height{600};
    bool is_initialized{false};
    uint64_t frame_count{0};
    float clear_color[4]{0.05f, 0.05f, 0.1f, 1.0f};

    bool is_ready() const {
#ifdef __EMSCRIPTEN__
        return is_initialized && device != nullptr && pipeline != nullptr && surface != nullptr;
#else
        return is_initialized;
#endif
    }
};

} // namespace graphics
} // namespace oasis

// Heap-allocated Flecs world to prevent stack use-after-free before async callbacks fire
static flecs::world* g_ecs = nullptr;

// Monotonic simulation frame counter
static uint64_t g_sim_tick = 0;

// Global UHAI Telemetry Channel (Producer mode)
static oasis::uhai::UhaiTelemetryChannel g_telemetry_channel(oasis::uhai::UhaiTelemetryChannel::ChannelMode::Producer);

// Write telemetry frame sample on each engine tick
static void TickTelemetry() {
    auto now = std::chrono::steady_clock::now();
    uint64_t current_time_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();

    static auto s_last_frame_time = now;
    auto frame_delta_us = std::chrono::duration_cast<std::chrono::microseconds>(now - s_last_frame_time).count();
    s_last_frame_time = now;
    float calculated_fps = (frame_delta_us > 0) ? (1000000.0f / static_cast<float>(frame_delta_us)) : 60.0f;
    if (calculated_fps > 300.0f || calculated_fps < 1.0f) {
        calculated_fps = 60.0f;
    }

    float founder_stress = 0.25f;
    uint32_t memory_slot_count = 0;
    if (g_ecs) {
        auto founder = g_ecs->entity("Founder");
        if (founder.has<oasis::psychology::PsychologyComponent>()) {
            const auto& psych = founder.get<oasis::psychology::PsychologyComponent>();
            founder_stress = psych.stress;
            memory_slot_count = psych.memory_ring.count;
        }
    }

    oasis::uhai::TelemetrySample sample{};
    sample.frame_count = g_sim_tick;
    sample.timestamp_ns = current_time_ns;
    sample.fps = calculated_fps;
    sample.founder_stress = founder_stress;
    sample.memory_slot_count = memory_slot_count;
    sample.biome_id = 1; // 1 = Suburban Sprawl ("Lot 402 & Cul-de-sac")
    sample.quality_flags = 0x01; // Bit 0: Valid frame telemetry

    g_telemetry_channel.Push(sample);
}

#ifdef __EMSCRIPTEN__
void SetupPipeline(oasis::graphics::RenderState& state) {
    const std::string_view shaderCode = oasis::shaders::RAYMARCH_WGSL;

    wgpu::ShaderSourceWGSL wgslDesc{};
    wgslDesc.code = wgpu::StringView{shaderCode.data(), shaderCode.size()};

    wgpu::ShaderModuleDescriptor shaderModuleDesc{};
    shaderModuleDesc.nextInChain = &wgslDesc;
    wgpu::ShaderModule shaderModule = state.device.CreateShaderModule(&shaderModuleDesc);

    // Explicit empty pipeline layout (0 bind groups)
    wgpu::PipelineLayoutDescriptor plDesc{};
    plDesc.bindGroupLayoutCount = 0;
    plDesc.bindGroupLayouts = nullptr;
    wgpu::PipelineLayout pipelineLayout = state.device.CreatePipelineLayout(&plDesc);

    wgpu::ColorTargetState colorTarget{};
    colorTarget.format = state.format;
    colorTarget.writeMask = wgpu::ColorWriteMask::All;

    wgpu::FragmentState fragmentState{};
    fragmentState.module = shaderModule;
    fragmentState.entryPoint = wgpu::StringView{"fs_main"};
    fragmentState.targetCount = 1;
    fragmentState.targets = &colorTarget;

    wgpu::PrimitiveState primitiveState{};
    primitiveState.topology = wgpu::PrimitiveTopology::TriangleList;
    primitiveState.cullMode = wgpu::CullMode::None; // Never cull full-screen quad!
    primitiveState.frontFace = wgpu::FrontFace::CCW;

    wgpu::RenderPipelineDescriptor pipelineDesc{};
    pipelineDesc.layout = pipelineLayout;
    pipelineDesc.vertex.module = shaderModule;
    pipelineDesc.vertex.entryPoint = wgpu::StringView{"vs_main"};
    pipelineDesc.fragment = &fragmentState;
    pipelineDesc.primitive = primitiveState;
    pipelineDesc.multisample.count = 1;
    pipelineDesc.multisample.mask = 0xFFFFFFFF;

    state.pipeline = state.device.CreateRenderPipeline(&pipelineDesc);
    spdlog::info("WebGPU Render Pipeline created successfully with cullMode = None");
}

void RenderWebGPUFrame(oasis::graphics::RenderState& state) {
    if (!state.is_ready()) return;

    wgpu::SurfaceTexture surfaceTexture;
    state.surface.GetCurrentTexture(&surfaceTexture);
    if (surfaceTexture.status != wgpu::SurfaceGetCurrentTextureStatus::SuccessOptimal &&
        surfaceTexture.status != wgpu::SurfaceGetCurrentTextureStatus::SuccessSuboptimal) {
        return;
    }
    if (!surfaceTexture.texture) return;

    wgpu::TextureView backbuffer = surfaceTexture.texture.CreateView();
    if (!backbuffer) return;

    wgpu::RenderPassColorAttachment colorAttachment{};
    colorAttachment.view = backbuffer;
    colorAttachment.clearValue = {
        state.clear_color[0],
        state.clear_color[1],
        state.clear_color[2],
        state.clear_color[3]
    };
    colorAttachment.loadOp = wgpu::LoadOp::Clear;
    colorAttachment.storeOp = wgpu::StoreOp::Store;

    wgpu::RenderPassDescriptor renderPassDesc{};
    renderPassDesc.colorAttachmentCount = 1;
    renderPassDesc.colorAttachments = &colorAttachment;

    wgpu::CommandEncoder encoder = state.device.CreateCommandEncoder();
    wgpu::RenderPassEncoder pass = encoder.BeginRenderPass(&renderPassDesc);
    pass.SetPipeline(state.pipeline);
    pass.Draw(3, 1, 0, 0); // 3 vertices, 1 instance (full-screen triangle)
    pass.End();

    wgpu::CommandBuffer commands = encoder.Finish();
    state.queue.Submit(1, &commands);

    state.frame_count++;
}

void InitWebGPUAsync(uint32_t width, uint32_t height) {
    wgpu::EmscriptenSurfaceSourceCanvasHTMLSelector canvasDesc{};
    canvasDesc.selector = wgpu::StringView{"#canvas"};

    wgpu::SurfaceDescriptor surfaceDesc{};
    surfaceDesc.nextInChain = &canvasDesc;

    wgpu::Instance instance = wgpu::CreateInstance();
    wgpu::Surface surface = instance.CreateSurface(&surfaceDesc);

    wgpu::RequestAdapterOptions adapterOpts{};
    adapterOpts.compatibleSurface = surface;

    instance.RequestAdapter(&adapterOpts, wgpu::CallbackMode::AllowSpontaneous,
        [instance, surface, width, height](wgpu::RequestAdapterStatus status, wgpu::Adapter adapter, wgpu::StringView message) {
            if (status != wgpu::RequestAdapterStatus::Success) {
                spdlog::error("Failed to acquire WebGPU adapter: {}", 
                              message.data ? std::string_view(message.data, message.length) : "Unknown");
                return;
            }

            wgpu::DeviceDescriptor deviceDesc{};
            deviceDesc.SetUncapturedErrorCallback(
                [](const wgpu::Device&, wgpu::ErrorType errorType, wgpu::StringView msg) {
                    spdlog::error("WebGPU Device Error (type {}): {}", 
                                  static_cast<int>(errorType), 
                                  msg.data ? std::string_view(msg.data, msg.length) : "Unknown");
                });

            adapter.RequestDevice(&deviceDesc, wgpu::CallbackMode::AllowSpontaneous,
                [instance, surface, adapter, width, height](wgpu::RequestDeviceStatus devStatus, wgpu::Device device, wgpu::StringView devMessage) {
                    if (devStatus != wgpu::RequestDeviceStatus::Success) {
                        spdlog::error("Failed to acquire WebGPU device: {}", 
                                      devMessage.data ? std::string_view(devMessage.data, devMessage.length) : "Unknown");
                        return;
                    }

                    wgpu::Queue queue = device.GetQueue();

                    // Query surface capabilities for preferred format
                    wgpu::SurfaceCapabilities capabilities{};
                    surface.GetCapabilities(adapter, &capabilities);
                    wgpu::TextureFormat preferredFormat = (capabilities.formatCount > 0)
                        ? capabilities.formats[0]
                        : wgpu::TextureFormat::BGRA8Unorm;

                    spdlog::info("Configuring WebGPU surface with format: {}", static_cast<int>(preferredFormat));

                    wgpu::SurfaceConfiguration config{};
                    config.device = device;
                    config.format = preferredFormat;
                    config.usage = wgpu::TextureUsage::RenderAttachment | wgpu::TextureUsage::CopySrc;
                    config.width = width;
                    config.height = height;
                    config.alphaMode = wgpu::CompositeAlphaMode::Auto;
                    config.presentMode = wgpu::PresentMode::Fifo;
                    surface.Configure(&config);

                    // Update RenderState component in Flecs
                    if (g_ecs) {
                        oasis::graphics::RenderState state{};
                        state.instance = instance;
                        state.adapter = adapter;
                        state.device = device;
                        state.queue = queue;
                        state.surface = surface;
                        state.format = preferredFormat;
                        state.width = width;
                        state.height = height;

                        SetupPipeline(state);

                        state.is_initialized = true;
                        g_ecs->entity("Renderer").set<oasis::graphics::RenderState>(state);
                        spdlog::info("WebGPU initialization complete and RenderState updated in Flecs");
                    }
                });
        });
}

void EngineTick(void* arg) {
    if (g_ecs) {
        g_ecs->progress();
        TickTelemetry();
    }
}
#endif

int main() {
    spdlog::info("Oasis V4 Engine Starting...");

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        spdlog::error("SDL_Init Error: {}", SDL_GetError());
        return 1;
    }

    constexpr uint32_t kWidth = 800;
    constexpr uint32_t kHeight = 600;

    SDL_Window* window = SDL_CreateWindow(
        "Oasis V4",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        kWidth, kHeight,
        SDL_WINDOW_SHOWN
    );
    if (!window) {
        spdlog::error("SDL_CreateWindow Error: {}", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Allocate Flecs world on the heap to prevent stack deallocation / UAF
    g_ecs = new flecs::world();
    g_ecs->set_target_fps(60.0f);

    // Register psychological components
    g_ecs->component<oasis::psychology::PersonalityFacets>();
    g_ecs->component<oasis::psychology::PsychologyComponent>();

    oasis::psychology::PsychologyComponent founder_psych{};
    founder_psych.facets = {0.8f, 0.9f, 0.6f, 0.7f, 0.3f};
    founder_psych.stress = 0.25f;
    founder_psych.mood = 0.50f;
    founder_psych.focus = 0.75f;

    // Seed initial Genesis Awakening formative memory (tick 0)
    oasis::psychology::EpisodicMemoryNode m0{};
    m0.timestamp_tick = 0;
    m0.event_type = static_cast<uint16_t>(oasis::psychology::MemoryEventType::GENESIS_AWAKENING);
    m0.emotional_valence = 40;
    m0.salience = 65;
    m0.flags = oasis::psychology::MemoryFlags::NONE;
    std::strncpy(m0.description, "Genesis", sizeof(m0.description) - 1);
    uint32_t initial_slot = founder_psych.memory_ring.Push(m0);

    spdlog::info("[Oasis Cognitive Core] Founder Memory: tick={} event={} salience={} stress={:.2f} (Slot {}/32, Permanent={})",
                 0ULL, m0.description, m0.salience, founder_psych.stress, initial_slot, founder_psych.memory_ring.permanent_count);

    g_ecs->entity("Founder")
        .set<oasis::psychology::PersonalityFacets>(founder_psych.facets)
        .set<oasis::psychology::PsychologyComponent>(founder_psych);

    // Register FounderPsychologySystem in flecs::OnUpdate phase
    g_ecs->system<oasis::psychology::PsychologyComponent>("FounderPsychologySystem")
        .kind(flecs::OnUpdate)
        .each([](oasis::psychology::PsychologyComponent& psych) {
            g_sim_tick++;
            const uint64_t tick = g_sim_tick;

            bool recorded = false;
            oasis::psychology::EpisodicMemoryNode node{};
            node.timestamp_tick = tick;

            if (tick == 1) {
                node.event_type = static_cast<uint16_t>(oasis::psychology::MemoryEventType::MUNICIPAL_CODE_RAID);
                node.emotional_valence = -90;
                node.salience = 95; // salience >= 80 -> sets PERMANENT_MEMORY
                node.flags = oasis::psychology::MemoryFlags::TRAUMA;
                std::strncpy(node.description, "Raid", sizeof(node.description) - 1);
                psych.stress = 0.65f;
                psych.mood = 0.20f;
                recorded = true;
            } else if (tick == 10) {
                node.event_type = static_cast<uint16_t>(oasis::psychology::MemoryEventType::SIPHON_POWER_GRID);
                node.emotional_valence = 50;
                node.salience = 75;
                std::strncpy(node.description, "Power", sizeof(node.description) - 1);
                psych.stress = 0.55f;
                psych.mood = 0.40f;
                recorded = true;
            } else if (tick == 20) {
                node.event_type = static_cast<uint16_t>(oasis::psychology::MemoryEventType::WATER_MAIN_TAPPED);
                node.emotional_valence = 45;
                node.salience = 65;
                std::strncpy(node.description, "Water", sizeof(node.description) - 1);
                psych.stress = 0.45f;
                psych.mood = 0.50f;
                recorded = true;
            } else if (tick == 25) {
                node.event_type = static_cast<uint16_t>(oasis::psychology::MemoryEventType::FIRST_KILOWATT_GENERATED);
                node.emotional_valence = 85;
                node.salience = 90; // salience >= 80 -> sets PERMANENT_MEMORY
                node.flags = oasis::psychology::MemoryFlags::EUPHORIA;
                std::strncpy(node.description, "Solar", sizeof(node.description) - 1);
                psych.stress = 0.30f;
                psych.mood = 0.70f;
                recorded = true;
            } else if (tick == 35) {
                node.event_type = static_cast<uint16_t>(oasis::psychology::MemoryEventType::SOLAR_INVERTER_REPAIR);
                node.emotional_valence = 30;
                node.salience = 55;
                std::strncpy(node.description, "Invert", sizeof(node.description) - 1);
                psych.stress = 0.28f;
                psych.mood = 0.72f;
                recorded = true;
            } else if (tick == 45) {
                node.event_type = static_cast<uint16_t>(oasis::psychology::MemoryEventType::TOOL_DROPPED);
                node.emotional_valence = -15;
                node.salience = 25;
                std::strncpy(node.description, "Tool", sizeof(node.description) - 1);
                psych.stress = 0.32f;
                psych.mood = 0.68f;
                recorded = true;
            } else if (tick == 55) {
                node.event_type = static_cast<uint16_t>(oasis::psychology::MemoryEventType::COMMUNAL_FEAST);
                node.emotional_valence = 80;
                node.salience = 85; // salience >= 80 -> sets PERMANENT_MEMORY
                node.flags = oasis::psychology::MemoryFlags::EUPHORIA;
                std::strncpy(node.description, "Feast", sizeof(node.description) - 1);
                psych.stress = 0.22f;
                psych.mood = 0.85f;
                recorded = true;
            } else if (tick > 60 && (tick % 15 == 0)) {
                uint32_t cycle = (tick / 15) % 5;
                if (cycle == 0) {
                    node.event_type = static_cast<uint16_t>(oasis::psychology::MemoryEventType::HOA_VIOLATION_NOTICE);
                    node.emotional_valence = -60;
                    node.salience = 82; // salience >= 80 -> sets PERMANENT_MEMORY
                    std::strncpy(node.description, "HOA", sizeof(node.description) - 1);
                    psych.stress = std::min(1.0f, psych.stress + 0.15f);
                } else if (cycle == 1) {
                    node.event_type = static_cast<uint16_t>(oasis::psychology::MemoryEventType::STRANGE_MOOD_MASTERWORK);
                    node.emotional_valence = 95;
                    node.salience = 88; // salience >= 80 -> sets PERMANENT_MEMORY
                    node.flags = oasis::psychology::MemoryFlags::EUPHORIA;
                    std::strncpy(node.description, "Master", sizeof(node.description) - 1);
                    psych.mood = std::min(1.0f, psych.mood + 0.20f);
                } else if (cycle == 2) {
                    node.event_type = static_cast<uint16_t>(oasis::psychology::MemoryEventType::FREEZING_WINTER_NIGHT);
                    node.emotional_valence = -40;
                    node.salience = 70;
                    std::strncpy(node.description, "Freeze", sizeof(node.description) - 1);
                    psych.stress = std::min(1.0f, psych.stress + 0.08f);
                } else if (cycle == 3) {
                    node.event_type = static_cast<uint16_t>(oasis::psychology::MemoryEventType::TOOL_DROPPED);
                    node.emotional_valence = -10;
                    node.salience = 20;
                    std::strncpy(node.description, "DropTool", sizeof(node.description) - 1);
                } else {
                    node.event_type = static_cast<uint16_t>(oasis::psychology::MemoryEventType::SIPHON_POWER_GRID);
                    node.emotional_valence = 40;
                    node.salience = 60;
                    std::strncpy(node.description, "TapGrid", sizeof(node.description) - 1);
                    psych.stress = std::max(0.1f, psych.stress - 0.05f);
                }
                recorded = true;
            }

            if (recorded) {
                uint32_t slot = psych.memory_ring.Push(node);
                spdlog::info("[Oasis Cognitive Core] Founder Memory: tick={} event={} salience={} stress={:.2f} (Slot {}/32, Permanent={})",
                             tick, node.description, node.salience, psych.stress, slot, psych.memory_ring.permanent_count);
            }

            // Periodic diagnostic memory summary every 60 ticks
            if (tick % 60 == 0) {
                spdlog::info("[Oasis Cognitive Core] Diagnostic Summary: tick={} | Total Recorded: {} | Active Slots: {}/32 | Permanent: {} | Stress: {:.2f} | Mood: {:.2f} | Focus: {:.2f}",
                             tick, psych.memory_ring.total_recorded, psych.memory_ring.count, psych.memory_ring.permanent_count,
                             psych.stress, psych.mood, psych.focus);
            }
        });

    // Register Renderer entity with non-zero size RenderState
    g_ecs->component<oasis::graphics::RenderState>();
    oasis::graphics::RenderState initial_state{};
    initial_state.width = kWidth;
    initial_state.height = kHeight;
    g_ecs->entity("Renderer").set<oasis::graphics::RenderState>(initial_state);

#ifdef __EMSCRIPTEN__
    // Integrate WebGPU render pass into the Flecs system loop
    g_ecs->system<oasis::graphics::RenderState>("WebGPURenderPass")
        .kind(flecs::OnStore)
        .each([](oasis::graphics::RenderState& state) {
            RenderWebGPUFrame(state);
        });

    // Initiate modern asynchronous WebGPU adapter & device request flow
    InitWebGPUAsync(kWidth, kHeight);

    // Set up main loop via emscripten_set_main_loop_arg to tick g_ecs->progress()
    emscripten_set_main_loop_arg(EngineTick, g_ecs, 0, false);
#else
    g_ecs->system<oasis::graphics::RenderState>("NativeStubRenderPass")
        .kind(flecs::OnStore)
        .each([](oasis::graphics::RenderState& state) {
            state.frame_count++;
        });

    int ticks = 0;
    while (g_ecs->progress()) {
        TickTelemetry();
        ticks++;
        if (ticks >= 60) break;
    }
#endif

#ifndef __EMSCRIPTEN__
    SDL_DestroyWindow(window);
    SDL_Quit();
    delete g_ecs;
    g_ecs = nullptr;
#endif

    return 0;
}
