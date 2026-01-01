#include "Render.hpp"
#include "util.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <mutex>
#include <algorithm>
#include <iostream>
#include <memory>
#include <future>
#include <thread>

// --- Internal Structures ---

struct RenderRequest {
    std::vector<std::string> paths;
    std::string title;
};

// Thread-safe queue
static std::vector<RenderRequest> requestQueue;
static std::mutex queueMutex;

// Helper function to load surface asynchronously
static SDL_Surface* loadSurfaceAsync(std::string path) {
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == nullptr) {
        std::cerr << "Unable to load image " << path << "! SDL_image Error: " << IMG_GetError() << std::endl;
    }
    return loadedSurface;
}

struct ImageState {
    std::string path;
    std::shared_future<SDL_Surface*> surfaceFuture;
    SDL_Texture* texture = nullptr;
    bool textureCreated = false;
};

struct ImageWindow {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    Uint32 windowID = 0;
    bool shouldClose = false;
    
    std::vector<ImageState> images;
    size_t currentIndex = 0;

    ~ImageWindow() {
        for(auto& img : images) {
            if (img.texture) SDL_DestroyTexture(img.texture);
        }
        if (renderer) SDL_DestroyRenderer(renderer);
        if (window) SDL_DestroyWindow(window);
    }
};

// --- Public API ---

void requestRender(const std::vector<std::string>& imagePaths, const std::string& title) {
    std::lock_guard<std::mutex> lock(queueMutex);
    requestQueue.push_back({imagePaths, title});
}

void runRenderLoop(std::atomic<bool>& isAppRunning) {
    // 1. Global Initialization
    
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL Init Failed: " << SDL_GetError() << std::endl;
        return;
    }
    int imgFlags = (IMG_INIT_PNG | IMG_INIT_JPG);
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        std::cerr << "SDL_image Init Failed: " << IMG_GetError() << std::endl;
        return;
    }

    std::vector<std::unique_ptr<ImageWindow>> windows;

    // 2. The Main UI Loop
    while (isAppRunning || !windows.empty()) {
        
        // A. Process New Requests
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            for (const auto& req : requestQueue) {
                if (req.paths.empty()) continue;

                auto win = std::make_unique<ImageWindow>();
                // Create window
                win->window = SDL_CreateWindow(req.title.c_str(), 
                                             SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
                                             800, 600, 
                                             SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
                
                if (win->window) {
                    win->windowID = SDL_GetWindowID(win->window);
                    win->renderer = SDL_CreateRenderer(win->window, -1, SDL_RENDERER_ACCELERATED);
                    
                    if (win->renderer) {
                        // Initialize Image States and start async loading
                        win->images.resize(req.paths.size());
                        for(size_t i=0; i<req.paths.size(); ++i) {
                            win->images[i].path = req.paths[i];
                            win->images[i].surfaceFuture = std::async(std::launch::async, loadSurfaceAsync, req.paths[i]);
                        }
                        windows.push_back(std::move(win));
                    }
                }
            }
            requestQueue.clear();
        }

        // B. Handle SDL Events
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                // Don't quit the app, just close all windows
                for (auto& win : windows) {
                    win->shouldClose = true;
                }
            }
            else if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE) {
                for (auto& win : windows) {
                    if (win->windowID == e.window.windowID) {
                        win->shouldClose = true;
                    }
                }
            }
            else if (e.type == SDL_KEYDOWN) {
                // Find which window has focus or matches the event windowID
                for (auto& win : windows) {
                    if (win->windowID == e.key.windowID) {
                        switch (e.key.keysym.sym) {
                            case SDLK_RIGHT:
                                win->currentIndex = (win->currentIndex + 1) % win->images.size();
                                break;
                            case SDLK_LEFT:
                                win->currentIndex = (win->currentIndex - 1 + win->images.size()) % win->images.size();
                                break;
                            case SDLK_ESCAPE:
                            case SDLK_q:
                                win->shouldClose = true;
                                break;
                        }
                        break; // Found the window
                    }
                }
            }
        }

        // C. Update & Render All Windows
        for (auto& win : windows) {
            if (!win->renderer) continue;

            // Check texture for current image
            ImageState& currentImg = win->images[win->currentIndex];
            if (!currentImg.textureCreated) {
                if (currentImg.surfaceFuture.valid()) {
                    auto status = currentImg.surfaceFuture.wait_for(std::chrono::milliseconds(0));
                    if (status == std::future_status::ready) {
                        SDL_Surface* surface = currentImg.surfaceFuture.get();
                        if (surface) {
                            currentImg.texture = SDL_CreateTextureFromSurface(win->renderer, surface);
                            SDL_FreeSurface(surface);
                        }
                        currentImg.textureCreated = true;
                    }
                }
            }

            // Draw
            SDL_SetRenderDrawColor(win->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderClear(win->renderer);

            if (currentImg.textureCreated && currentImg.texture) {
                SDL_RenderCopy(win->renderer, currentImg.texture, NULL, NULL);
            } else {
                // Loading placeholder
                SDL_SetRenderDrawColor(win->renderer, 0x80, 0x80, 0x80, 0xFF);
                SDL_Rect rect = { 400 - 50, 300 - 50, 100, 100 }; // Center-ish
                SDL_RenderFillRect(win->renderer, &rect);
            }

            SDL_RenderPresent(win->renderer);
        }

        // D. Cleanup Closed Windows
        windows.erase(std::remove_if(windows.begin(), windows.end(), 
            [](const std::unique_ptr<ImageWindow>& w){ return w->shouldClose; }), 
            windows.end());

        // E. Sleep
        SDL_Delay(16);
    }

    // 3. Global Cleanup
    windows.clear();
    IMG_Quit();
    SDL_Quit();
}
