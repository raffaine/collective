#pragma once
#include <SDL2/SDL.h>
#include <functional>
#include <string>

namespace oasis {

// Story 3.3: Cross-platform deployment shell for Native and WASM
class SDL2Abstraction {
public:
    SDL2Abstraction(const std::string& title, int width, int height);
    ~SDL2Abstraction();

    bool Initialize();
    
    // Begins the main loop. Uses emscripten_set_main_loop for WASM, while() for native.
    void Run(std::function<void()> tick_callback, std::function<void(SDL_Renderer*)> render_callback);

    void Quit();

private:
    std::string title_;
    int width_, height_;
    SDL_Window* window_ = nullptr;
    SDL_Renderer* renderer_ = nullptr;
    bool running_ = false;

    // Callbacks stored for the static C-wrapper required by Emscripten
    std::function<void()> current_tick_;
    std::function<void(SDL_Renderer*)> current_render_;

    static void LoopIteration(void* arg);
};

} // namespace oasis
