#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <memory>
#include <string>
#include <string_view>
#include <utility>

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpadded"
#endif
#include <spdlog/spdlog.h>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
#if defined(__clang__)
#pragma clang diagnostic pop
#endif

#include "engine/app/oasis_application.hpp"

namespace {

[[nodiscard]] uint64_t ParsePositiveU64OrDefault(std::string_view text, uint64_t fallback) noexcept {
    if (text.empty()) {
        return fallback;
    }
    const std::string buf(text);
    char*             end_ptr = nullptr;
    const unsigned long long parsed = std::strtoull(buf.c_str(), &end_ptr, 10);
    if (end_ptr == buf.c_str() || *end_ptr != '\0' || parsed == 0ULL) {
        return fallback;
    }
    return static_cast<uint64_t>(parsed);
}

[[nodiscard]] oasis::engine::app::OasisAppConfig ParseCliArguments(int argc, char* argv[]) {
    oasis::engine::app::OasisAppConfig config{};

    if (const char* env_headless = std::getenv("OASIS_HEADLESS_CI");
        env_headless != nullptr && std::string_view(env_headless) == "1") {
        config.headless_ci = true;
    }

    for (int i = 1; i < argc; ++i) {
        if (argv[i] == nullptr) {
            continue;
        }
        const std::string_view arg(argv[i]);

        if (arg == "--headless-ci") {
            config.headless_ci = true;
        } else if (arg == "--procedural-map") {
            config.procedural_map = true;
        } else if (arg == "--max-ticks" && (i + 1) < argc && argv[i + 1] != nullptr) {
            config.max_ticks = ParsePositiveU64OrDefault(argv[++i], config.max_ticks);
        } else if (arg.starts_with("--max-ticks=")) {
            config.max_ticks = ParsePositiveU64OrDefault(arg.substr(12u), config.max_ticks);
        } else if (arg == "--map" && (i + 1) < argc && argv[i + 1] != nullptr) {
            config.map_path = std::string(argv[++i]);
        } else if (arg.starts_with("--map=")) {
            config.map_path = std::string(arg.substr(6u));
        } else if (arg == "--mod" && (i + 1) < argc && argv[i + 1] != nullptr) {
            config.mod_paths.emplace_back(argv[++i]);
        } else if (arg.starts_with("--mod=")) {
            config.mod_paths.emplace_back(arg.substr(6u));
        }
    }

    return config;
}

} // namespace

int main(int argc, char* argv[]) {
    spdlog::set_level(spdlog::level::info);
    oasis::engine::app::OasisAppConfig config = ParseCliArguments(argc, argv);

#ifdef __EMSCRIPTEN__
    auto* app = new oasis::engine::app::OasisApplication(std::move(config));
    if (!app->Initialize()) {
        spdlog::error("[Oasis Engine] Failed to initialize modular WebGPU application.");
        delete app;
        return 1;
    }
    emscripten_set_main_loop_arg(
        &oasis::engine::app::OasisApplication::EmscriptenMainLoopCallback,
        app,
        0,
        false
    );
    return 0;
#else
    auto app = std::make_unique<oasis::engine::app::OasisApplication>(std::move(config));
    if (!app->Initialize()) {
        spdlog::error("[Oasis Engine] Failed to initialize modular application.");
        return 1;
    }
    return app->RunMainLoop();
#endif
}
