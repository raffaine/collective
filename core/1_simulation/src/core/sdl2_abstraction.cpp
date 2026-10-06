#include "sdl2_abstraction.hpp"
#include <iostream>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

namespace oasis {

SDL2Abstraction::SDL2Abstraction(const std::string& title, int width, int height)
    : title_(title), width_(width), height_(height) {}

SDL2Abstraction::~SDL2Abstraction() {
    Quit();
}

bool SDL2Abstraction::Initialize() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << "\n";
        return false;
    }

    window_ = SDL_CreateWindow(title_.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width_, height_, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (!window_) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << "\n";
        return false;
    }

    renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer_) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << "\n";
        return false;
    }

    running_ = true;
    return true;
}

void SDL2Abstraction::LoopIteration(void* arg) {
    SDL2Abstraction* app = static_cast<SDL2Abstraction*>(arg);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            app->running_ = false;
#ifdef __EMSCRIPTEN__
            emscripten_cancel_main_loop();
#endif
        }
    }

    // Engine Tick
    if (app->current_tick_) {
        app->current_tick_();
    }

    // Engine Render
    SDL_SetRenderDrawColor(app->renderer_, 46, 125, 50, 255); // Sovereign Green
    SDL_RenderClear(app->renderer_);
    
    if (app->current_render_) {
        app->current_render_(app->renderer_);
    }
    
    SDL_RenderPresent(app->renderer_);
}

void SDL2Abstraction::Run(std::function<void()> tick_callback, std::function<void(SDL_Renderer*)> render_callback) {
    current_tick_ = tick_callback;
    current_render_ = render_callback;

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(LoopIteration, this, 0, 1);
#else
    while (running_) {
        LoopIteration(this);
        SDL_Delay(16); // ~60 FPS
    }
#endif
}

void SDL2Abstraction::Quit() {
    if (renderer_) {
        SDL_DestroyRenderer(renderer_);
        renderer_ = nullptr;
    }
    if (window_) {
        SDL_DestroyWindow(window_);
        window_ = nullptr;
    }
    SDL_Quit();
}

} // namespace oasis
