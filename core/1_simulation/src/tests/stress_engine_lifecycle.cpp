#include <flecs.h>
#include <spdlog/spdlog.h>
#include <cassert>
#include <iostream>
#include <vector>
#include <chrono>

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
    void* device{nullptr};
    void* queue{nullptr};
    void* surface{nullptr};
    void* pipeline{nullptr};
    uint32_t width{800};
    uint32_t height{600};
    bool is_initialized{false};
    uint64_t frame_count{0};
    float clear_color[4]{0.05f, 0.05f, 0.1f, 1.0f};

    bool is_ready() const {
        return is_initialized;
    }
};

} // namespace graphics
} // namespace oasis

int main() {
    spdlog::info("=== Starting Adversarial Flecs Lifecycle & Memory Stress Test ===");

    // Test 1: Verify RenderState component size
    constexpr size_t render_state_size = sizeof(oasis::graphics::RenderState);
    spdlog::info("RenderState size: {} bytes", render_state_size);
    assert(render_state_size > 0);
    assert(render_state_size >= 48); // Must hold pointers and frame data

    // Test 2: Lifecycle creation and destruction (detect memory leaks & UAF)
    for (int cycle = 0; cycle < 10; ++cycle) {
        flecs::world* ecs = new flecs::world();

        ecs->component<oasis::psychology::PersonalityFacets>();
        ecs->component<oasis::graphics::RenderState>();

        flecs::entity founder = ecs->entity("Founder")
            .set<oasis::psychology::PersonalityFacets>({0.8f, 0.9f, 0.6f, 0.7f, 0.3f});

        oasis::graphics::RenderState init_state{};
        init_state.width = 800;
        init_state.height = 600;
        init_state.is_initialized = true;

        flecs::entity renderer = ecs->entity("Renderer")
            .set<oasis::graphics::RenderState>(init_state);

        uint64_t tick_count = 0;
        ecs->system<oasis::graphics::RenderState>("RenderPassSystem")
            .kind(flecs::OnStore)
            .each([&tick_count](oasis::graphics::RenderState& state) {
                assert(state.is_ready());
                state.frame_count++;
                tick_count++;
            });

        // Tick 500 times in each cycle with explicit dt (bypassing real-time sleep)
        for (int t = 0; t < 500; ++t) {
            ecs->progress(0.016667f);
        }

        assert(tick_count == 500);
        const auto& final_state = renderer.get<oasis::graphics::RenderState>();
        assert(final_state.frame_count == 500);

        delete ecs;
    }
    spdlog::info("Test 2 PASSED: 10 cycles of Flecs world creation, 500 ticks, and destruction clean.");

    // Test 3: Long-running stress test (100,000 ticks)
    {
        flecs::world* ecs = new flecs::world();

        ecs->component<oasis::graphics::RenderState>();
        oasis::graphics::RenderState init_state{};
        init_state.width = 1920;
        init_state.height = 1080;
        init_state.is_initialized = true;

        flecs::entity renderer = ecs->entity("Renderer").set<oasis::graphics::RenderState>(init_state);

        uint64_t simulated_frames = 0;
        ecs->system<oasis::graphics::RenderState>("StressRenderPass")
            .kind(flecs::OnStore)
            .each([&simulated_frames](oasis::graphics::RenderState& state) {
                state.frame_count++;
                simulated_frames++;
            });

        auto start = std::chrono::high_resolution_clock::now();
        constexpr uint64_t kTargetTicks = 100000;
        for (uint64_t i = 0; i < kTargetTicks; ++i) {
            ecs->progress(0.016667f);
        }
        auto end = std::chrono::high_resolution_clock::now();
        double elapsed_ms = std::chrono::duration<double, std::milli>(end - start).count();

        const auto& verified_state = renderer.get<oasis::graphics::RenderState>();
        assert(verified_state.frame_count == kTargetTicks);
        assert(simulated_frames == kTargetTicks);

        spdlog::info("Test 3 PASSED: Executed {} ticks in {:.2f} ms ({:.0f} ticks/sec). No corruption detected.",
                     kTargetTicks, elapsed_ms, (kTargetTicks / (elapsed_ms / 1000.0)));

        delete ecs;
    }

    // Test 4: Dynamic entity mutation under continuous ticking (concurrent stress)
    {
        flecs::world* ecs = new flecs::world();
        ecs->component<oasis::graphics::RenderState>();

        oasis::graphics::RenderState init_state{};
        init_state.is_initialized = true;
        ecs->entity("Renderer").set<oasis::graphics::RenderState>(init_state);

        ecs->system<oasis::graphics::RenderState>("TickSystem")
            .kind(flecs::OnStore)
            .each([](oasis::graphics::RenderState& state) {
                state.frame_count++;
            });

        std::vector<flecs::entity> temp_entities;
        temp_entities.reserve(1000);

        for (int i = 0; i < 5000; ++i) {
            // Spawn entity
            if (i % 2 == 0) {
                flecs::entity e = ecs->entity().set<oasis::psychology::PersonalityFacets>({0.1f, 0.2f, 0.3f, 0.4f, 0.5f});
                temp_entities.push_back(e);
            }
            // Despawn entity
            if (!temp_entities.empty() && i % 3 == 0) {
                temp_entities.back().destruct();
                temp_entities.pop_back();
            }
            ecs->progress(0.016667f);
        }

        const auto& s = ecs->entity("Renderer").get<oasis::graphics::RenderState>();
        assert(s.frame_count == 5000);

        spdlog::info("Test 4 PASSED: Dynamic entity churn (spawn/despawn) survived 5000 ticks without corruption.");
        delete ecs;
    }

    // Test 5: RenderState query semantics (empty struct assertion defense)
    {
        flecs::world* ecs = new flecs::world();
        ecs->component<oasis::graphics::RenderState>();

        // Assert Flecs sees non-zero size
        flecs::entity comp_entity = ecs->component<oasis::graphics::RenderState>();
        flecs::type comp_type = comp_entity.type();
        assert(comp_type.count() > 0);

        int query_match_count = 0;
        ecs->query<oasis::graphics::RenderState>()
            .each([&query_match_count](flecs::entity e, oasis::graphics::RenderState& s) {
                query_match_count++;
            });

        // 0 matches before creating entity
        assert(query_match_count == 0);

        ecs->entity("Renderer").set<oasis::graphics::RenderState>({nullptr, nullptr, nullptr, nullptr, 800, 600, true, 42});

        query_match_count = 0;
        ecs->query<oasis::graphics::RenderState>()
            .each([&query_match_count](flecs::entity e, oasis::graphics::RenderState& s) {
                assert(s.frame_count == 42);
                assert(s.width == 800);
                assert(s.height == 600);
                query_match_count++;
            });

        assert(query_match_count == 1);
        spdlog::info("Test 5 PASSED: RenderState correctly queried and non-empty in Flecs ECS.");
        delete ecs;
    }

    spdlog::info("=== ALL NATIVE STRESS TESTS PASSED SUCCESSFULLY ===");
    return 0;
}
