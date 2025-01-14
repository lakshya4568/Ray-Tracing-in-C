#include <stdio.h>
#include </opt/homebrew/include/sdl2/SDL.h>


#define WIDTH 900  
#define HEIGHT 600
#define COLOR_WHITE 0xffffffff



int main()
{
    // Initialize SDL2 video subsystem; returns 0 on success, negative on failure
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL Initialization failed: %s\n", SDL_GetError());
        return 1;  
    }

    // Create an SDL window with specified title, position, size, and flags
    SDL_Window *window = SDL_CreateWindow(
        "Ray Tracing",          // Window title
        SDL_WINDOWPOS_CENTERED, // Initial x position: centered on screen
        SDL_WINDOWPOS_CENTERED, // Initial y position: centered on screen
        800,                    // Window width in pixels
        600,                    // Window height in pixels
        SDL_WINDOW_SHOWN        // Window is visible upon creation
    );

    // retrieve the surface in case we need to draw anything to the window (a sheet)
    SDL_Surface *surface = SDL_GetWindowSurface(window);
    SDL_Rect rect = (SDL_Rect){200, 200, 200, 200};
    // rect.h, rect.w, rect.x, rect.y = 200;
    SDL_FillRect(surface, &rect, COLOR_WHITE);

    // changes to the surface will update here 
    SDL_UpdateWindowSurface(window);

    // Check if window creation was successful
    if (!window) {
        // Print error message if window creation fails
        printf("Failed to create window: %s\n", SDL_GetError());
        SDL_Quit();  // Clean up initialized SDL subsystems
        return 1;    // Exit program with non-zero status indicating failure
    }



    // Declare an SDL event variable to handle events
    SDL_Event event;
    int running = 1;  // Flag to control the main loop; 1 means running

    // Enter the main event loop
    while (running) {
        // Process events in the queue
        while (SDL_PollEvent(&event)) {
            // Check if the event is a quit event (e.g., window close)
            if (event.type == SDL_QUIT) { 
                running = 0;  // Set running to 0 to exit the loop
            }
        }
        // Additional rendering and logic can be performed here

        SDL_Delay(16);  // Delay to control frame rate; ~16ms for ~60 FPS
    }

    // Destroy the created window to free resources
    SDL_DestroyWindow(window);
    SDL_Quit(); // Clean up all initialized SDL subsystems



    return 0;
}
