#include <flecs.h>
#include <spdlog/spdlog.h>
#include <SDL2/SDL.h>
#include <iostream>
#include <memory>
#include <string_view>
#include <cstring>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#include <webgpu/webgpu_cpp.h>
#endif

#include "shaders/raymarch_wgsl.hpp"

namespace oasis {
namespace psychology {
    struct PersonalityFacets {
        float openness;
        float conscientiousness;
        float extraversion;
        float agreeableness;
        float neuroticism;
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

    // Register psychological facets
    g_ecs->component<oasis::psychology::PersonalityFacets>();
    g_ecs->entity("Founder")
        .set<oasis::psychology::PersonalityFacets>({0.8f, 0.9f, 0.6f, 0.7f, 0.3f});

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
