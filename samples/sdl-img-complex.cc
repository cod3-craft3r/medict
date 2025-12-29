#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <iostream>
#include <memory>

// Structure to hold SDL resources
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

/**
 * Initialize SDL2 and create a window with renderer
 * @param width Window width
 * @param height Window height
 * @param title Window title
 * @return Unique pointer to SDLContext, or nullptr if initialization fails
 */
std::unique_ptr<SDLContext> initializeSDL(int width, int height, const std::string& title) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
        return nullptr;
    }
    
    auto context = std::make_unique<SDLContext>();
    
    context->window = SDL_CreateWindow(
        title.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width, height,
        SDL_WINDOW_SHOWN
    );
    
    if (!context->window) {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return nullptr;
    }
    
    context->renderer = SDL_CreateRenderer(
        context->window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    // Force the cursor to appear
    SDL_ShowCursor(SDL_ENABLE);
    
    if (!context->renderer) {
        std::cerr << "Renderer creation failed: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(context->window);
        SDL_Quit();
        return nullptr;
    }
    
    // Initialize SDL_image for loading PNG, JPG, etc.
    if (!(IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) & (IMG_INIT_PNG | IMG_INIT_JPG))) {
        std::cerr << "SDL_image initialization failed: " << IMG_GetError() << std::endl;
        SDL_DestroyRenderer(context->renderer);
        SDL_DestroyWindow(context->window);
        SDL_Quit();
        return nullptr;
    }

    std::cout << "SDL Video driver: " << SDL_GetCurrentVideoDriver() << std::endl;
    
    return context;
}

/**
 * Load an image from a file path
 * @param renderer SDL Renderer
 * @param imagePath Path to the image file
 * @return SDL_Texture pointer, or nullptr if loading fails
 */
SDL_Texture* loadImage(SDL_Renderer* renderer, const std::string& imagePath) {
    SDL_Surface* surface = IMG_Load(imagePath.c_str());
    
    if (!surface) {
        std::cerr << "Failed to load image '" << imagePath << "': " << IMG_GetError() << std::endl;
        return nullptr;
    }
    
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    
    if (!texture) {
        std::cerr << "Failed to create texture from surface: " << SDL_GetError() << std::endl;
        return nullptr;
    }
    
    return texture;
}

/**
 * Render an image to the screen
 * @param renderer SDL Renderer
 * @param texture Image texture to render
 * @param x X position (top-left)
 * @param y Y position (top-left)
 * @param width Width to render at (0 = original)
 * @param height Height to render at (0 = original)
 */
void renderImage(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, int width = 0, int height = 0) {
    if (!texture) {
        std::cerr << "Cannot render null texture" << std::endl;
        return;
    }
    
    SDL_Rect destRect;
    destRect.x = x;
    destRect.y = y;
    
    // Get original texture dimensions if width/height not specified
    if (width == 0 || height == 0) {
        SDL_QueryTexture(texture, nullptr, nullptr, &destRect.w, &destRect.h);
    } else {
        destRect.w = width;
        destRect.h = height;
    }
    
    if (SDL_RenderCopy(renderer, texture, nullptr, &destRect) < 0) {
        std::cerr << "Failed to render texture: " << SDL_GetError() << std::endl;
    }
}

/**
 * Present the rendered frame to the screen
 * @param renderer SDL Renderer
 */
void presentFrame(SDL_Renderer* renderer) {
    SDL_RenderPresent(renderer);
}

/**
 * Clear the renderer with a background color
 * @param renderer SDL Renderer
 * @param r Red component (0-255)
 * @param g Green component (0-255)
 * @param b Blue component (0-255)
 */
void clearScreen(SDL_Renderer* renderer, uint8_t r = 255, uint8_t g = 255, uint8_t b = 255) {
    SDL_SetRenderDrawColor(renderer, r, g, b, 255);
    SDL_RenderClear(renderer);
}

int main() {
    std::unique_ptr<SDLContext> ctx = initializeSDL(640, 480, "Gopher");
    SDL_Texture* imgTexture = loadImage(ctx->renderer, "my_image.png");

    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        clearScreen(ctx->renderer, 255, 255, 255); // White background
        renderImage(ctx->renderer, imgTexture, 0, 0);
        presentFrame(ctx->renderer);
    }
}