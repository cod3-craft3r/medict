#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h> // Required for PNG/JPG loading
#include <iostream>
#include <string>
#include <thread> // Needed for the example below

struct SDLContext {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    
    ~SDLContext() {
        if (renderer) {
            SDL_DestroyRenderer(renderer);
        }
        if (window) {
            SDL_DestroyWindow(window);
        }
        SDL_Quit();
    }
};

// Define a state struct to hold data between frames
struct RenderState {
    std::unique_ptr<SDLContext> sdl;
    SDL_Texture* texture = nullptr;
};


SDL_Texture* loadTexture(const std::string &imagePath, SDL_Renderer* renderer) {
    SDL_Texture* newTexture = nullptr;

    // Load image at specified path using SDL_image
    // IMG_LoadTexture creates a texture directly from a file
    newTexture = IMG_LoadTexture(renderer, imagePath.c_str());

    if (newTexture == nullptr) {
        std::cerr << "Failed to load image! SDL_image Error: " << IMG_GetError() << std::endl;
    }

    return newTexture;
}

std::unique_ptr<SDLContext> initializeSDL(const std::string& title) {
    // 1. Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
        return nullptr;
    }
    
    auto context = std::make_unique<SDLContext>();

    // 2. Create Window
    context->window = SDL_CreateWindow(title.c_str(),
                                          SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
                                          854, 480, 
                                          SDL_WINDOW_SHOWN);
    
    if (!context->window) {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return nullptr;
    }
    
    // 3. Create Renderer
    // This is the "painter" that draws to the window
    context->renderer = SDL_CreateRenderer(context->window, -1, SDL_RENDERER_ACCELERATED);
    
    if (!context->renderer) {
        std::cerr << "Renderer creation failed: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(context->window);
        SDL_Quit();
        return nullptr;
    }

    // 4. Initialize SDL_image (IMPORTANT for PNG/JPG)
    int imgFlags = (IMG_INIT_PNG | IMG_INIT_JPG);
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
        SDL_DestroyRenderer(context->renderer);
        SDL_DestroyWindow(context->window);
        SDL_Quit();
        return nullptr;
    }
    
    return context;
}

// 1. INIT: Sets up window and returns immediately
RenderState* Render_Init(const std::string &imagePath, const std::string &title) {
    RenderState* state = new RenderState();
    
    // Reuse your existing initializeSDL function
    state->sdl = initializeSDL(title);
    if (!state->sdl) {
        delete state;
        return nullptr;
    }

    state->texture = loadTexture(imagePath, state->sdl->renderer);
    SDL_ShowCursor(SDL_ENABLE);
    
    return state;
}

// 2. UPDATE: Processes ONE frame and returns immediately
// Returns 'false' if the user wants to quit (clicked X)
bool Render_Update(RenderState* state) {
    if (!state) return false;

    SDL_Event e;
    // Poll all pending events (Non-blocking!)
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            return false;
        }
        // Handle other inputs here (keyboard, mouse)
    }

    // Draw the frame
    SDL_SetRenderDrawColor(state->sdl->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(state->sdl->renderer);

    if (state->texture) {
        SDL_RenderCopy(state->sdl->renderer, state->texture, NULL, NULL);
    }

    SDL_RenderPresent(state->sdl->renderer);
    
    return true; // Keep running
}

// 3. CLEANUP
void Render_Close(RenderState* state) {
    if (state) {
        if (state->texture) SDL_DestroyTexture(state->texture);
        if (state->sdl) {
            SDL_DestroyRenderer(state->sdl->renderer);
            SDL_DestroyWindow(state->sdl->window);
        }
        IMG_Quit();
        SDL_Quit();
        delete state;
    }
}
// Your heavy program logic
void MyProgramFlow() {
    std::cout << "Program started..." << std::endl;
    
    // Simulate long work
    for (int i = 0; i < 10; i++) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "Working... " << i << std::endl;
    }
    
    std::cout << "Program finished!" << std::endl;
}

int main(int argc, char* argv[]) {
    // 1. Initialize Window (Must be on Main Thread for OS reasons)
    RenderState* ui = Render_Init("my_image.png", "non-blocking");

    // 2. Launch your logic in a separate thread
    std::thread worker(MyProgramFlow);

    // 3. Run the UI Loop on the Main Thread
    bool running = true;
    while (running) {
        // Update UI (Process events, draw frame)
        running = Render_Update(ui);
        
        // Optional: Add a small sleep to prevent 100% CPU usage if VSync is off
        std::this_thread::sleep_for(std::chrono::milliseconds(16)); 
    }

    // 4. Cleanup
    // If the UI closes, we might need to wait for the worker or signal it to stop
    if (worker.joinable()) {
        worker.join(); 
    }
    
    Render_Close(ui);
    return 0;
}
