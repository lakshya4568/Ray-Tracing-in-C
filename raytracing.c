#include <stdio.h>
#include <math.h>
#include </opt/homebrew/include/sdl2/SDL.h>

#define WIDTH 1200
#define HEIGHT 600
#define RAYS_NUMBER 500
#define COLOR_WHITE 0xffffffff
#define COLOR_BLACK 0x00000000
#define RAY_COLOR 0xf4d03f
#define RAY_THICKNESS 2

typedef struct 
{
    int x;
    int y;
    int r;
} Circle;

/**
 * @brief A structure to represent a ray in 2D space.
 *
 * This structure defines a ray with an origin point and a direction vector.
 * The origin represents the starting point of the ray, and the direction
 * represents the direction in which the ray is pointing.
 */
typedef struct
{
    /**
     * @brief Represents the angle of the ray in the ray tracing algorithm.
     * 
     * This variable holds the angle at which the ray is projected. It is used
     * to calculate the direction and trajectory of the ray as it interacts
     * with objects in the scene.
     */
    /* Origin coordinate of ray to end of ray*/
    double x_start, y_start;
    double angle;
} Ray;

// ...existing code...

/**
 * @brief Applies a basic box blur to soften edges on the given SDL surface.
 *
 * This function analyzes each pixel and its neighbors, computes an average,
 * and writes the result back. To keep it simple, we handle only RGB888 or ARGB8888 formats here.
 *
 * Note: For performance reasons, we'd typically enable/disable surface locking
 * and handle boundary conditions carefully.
 *
 * @param surface The SDL surface on which to apply anti-aliasing.
 */
void BasicAntialiasing(SDL_Surface *surface)
{
    // Lock surface if needed
    if (SDL_MUSTLOCK(surface))
    {
        if (SDL_LockSurface(surface) < 0)
        {
            return;
        }
    }

    // Basic dimension lookup
    int width = surface->w;
    int height = surface->h;

    Uint32 *pixels = (Uint32 *)surface->pixels;
    Uint32 *temp = (Uint32 *)malloc(width * height * sizeof(Uint32));
    if (!temp)
    {
        if (SDL_MUSTLOCK(surface))
            SDL_UnlockSurface(surface);
        return;
    }

    // Copy current data to temp
    memcpy(temp, pixels, width * height * sizeof(Uint32));

    // For each pixel, average with up to 8 neighbors
    for (int y = 1; y < height - 1; y++)
    {
        for (int x = 1; x < width - 1; x++)
        {
            // Accumulators for R, G, B
            int totalR = 0, totalG = 0, totalB = 0;

            // Sample 3x3 region
            for (int j = -1; j <= 1; j++)
            {
                for (int i = -1; i <= 1; i++)
                {
                    int idx = (y + j) * width + (x + i);
                    Uint32 p = temp[idx];

                    // Extract ARGB or RGB
                    Uint8 r = (p & 0x00FF0000) >> 16;
                    Uint8 g = (p & 0x0000FF00) >> 8;
                    Uint8 b = (p & 0x000000FF);

                    totalR += r;
                    totalG += g;
                    totalB += b;
                }
            }

            // Average color
            Uint8 avgR = (Uint8)(totalR / 9);
            Uint8 avgG = (Uint8)(totalG / 9);
            Uint8 avgB = (Uint8)(totalB / 9);

            // Write to surface
            Uint32 finalColor = (0xFF << 24) | (avgR << 16) | (avgG << 8) | (avgB);
            pixels[y * width + x] = finalColor;
        }
    }

    free(temp);

    // Unlock surface if we locked it
    if (SDL_MUSTLOCK(surface))
    {
        SDL_UnlockSurface(surface);
    }
}

/**
 * @brief Generates an array of rays originating from a given circle.
 *
 * This function calculates and stores rays originating from the center of the specified circle.
 * Each ray is defined by an angle and the circle's center coordinates.
 *
 * @param circle The circle from which the rays originate.
 * @param rays An array to store the generated rays. The size of the array should be RAYS_NUMBER.
 */
void generate_rays(Circle circle, Ray rays[RAYS_NUMBER]) {
    for (int i = 0; i < RAYS_NUMBER; i++)
    {
        double angle = ((double)i / RAYS_NUMBER * 2 * M_PI);
        Ray ray = {circle.x, circle.y, angle};
        rays[i] = ray;
        //printf("Angle: %f\n", angle);
    }
}

/**
 * @brief Renders rays on an SDL surface and checks for intersections with a given circle.
 *
 * This function takes an array of rays and renders them on the provided SDL surface.
 * Each ray is drawn from its starting point in the direction specified by its angle.
 * The function also checks if the rays intersect with a given circle (object) and stops
 * drawing the ray if an intersection is detected or if the ray reaches the edge of the screen.
 *
 * @param surface The SDL surface on which the rays will be drawn.
 * @param rays An array of rays to be rendered. The size of the array should be RAYS_NUMBER.
 * @param color The color used to draw the rays.
 * @param Object The circle object to check for ray intersections.
 */
void FillRays(SDL_Surface *surface, Ray rays[RAYS_NUMBER], Uint32 color, Circle Object) {

    double radius_squared = pow(Object.r, 2);

    for (int i = 0; i <  RAYS_NUMBER; i++)
    {
        Ray ray = rays[i];
        int end_of_screen = 0;
        int object_hit = 0;

        int step = 1;

        double x_draw = ray.x_start;
        double y_draw = ray.y_start;

        while (!end_of_screen && !object_hit)
        {
            x_draw += step * cos(ray.angle);
            y_draw += step * sin(ray.angle);

            SDL_Rect pixel = (SDL_Rect){x_draw, y_draw, RAY_THICKNESS, RAY_THICKNESS}; // create pixel for each increment in ray;
            SDL_FillRect(surface, &pixel, color);


            if (x_draw < 0 || x_draw > WIDTH) 
            {
                end_of_screen = 1;
            }

            if (y_draw < 0 || y_draw > HEIGHT)
            {
                end_of_screen = 1;
            }

            //does the ray hit the object, lets's check...
            double distance_squared = pow(x_draw - Object.x, 2) + pow(y_draw - Object.y, 2);
            if (distance_squared < radius_squared)
            {
                break;
            }
        }
    }
}

// Function for creation of a filled circle
void FillCircle(SDL_Surface *s, Circle circle, Uint32 color)
{
    // looping the pixel length, from x-r to x+r and same for y coordinate for each pixel
    double radius_squared = pow(circle.r, 2);
    for (double x = circle.x - circle.r; x <= circle.x + circle.r; x++)
    {
        for (double y = circle.y - circle.r; y <= circle.y + circle.r; y++)
        {
            double distance_squared = pow(x - circle.x, 2) + pow(y - circle.y, 2);
            if (distance_squared < radius_squared)
            {
                SDL_Rect pixel = (SDL_Rect){x, y, 1, 1}; // can be used to create the pixel
                SDL_FillRect(s, &pixel, color);
            }
        }
    }
}

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
        WIDTH,                    // Window width in pixels
        HEIGHT,                    // Window height in pixels
        0                // Window is visible upon creation
    );

    // retrieve the surface in case we need to draw anything to the window (a sheet)
    SDL_Surface *surface = SDL_GetWindowSurface(window);

    /* creation of a rectangle in sdl 
     SDL_Rect rect = (SDL_Rect){200, 200, 200, 200};
     rect.h, rect.w, rect.x, rect.y = 200;
     SDL_FillRect(surface, &rect, COLOR_WHITE);
    */

    Circle circle = {200, 200, 40};
    SDL_Rect erase_rect = (SDL_Rect){0, 0, WIDTH, HEIGHT};

    // shadow circle
    Circle shadow_circle = {550, 300, 120};
    Ray rays[RAYS_NUMBER];
    generate_rays(circle, rays);

    // Check if window creation was successful
    if (!window) {
        // Print error message if window creation fails
        printf("Failed to create window: %s\n", SDL_GetError());
        SDL_Quit();  // Clean up initialized SDL subsystems
        return 1;    // Exit program with non-zero status indicating failure
    }

    // Declare an SDL event variable to handle events for updation 
    SDL_Event event;
    int running = 1;  // Flag to control the main loop; 1 means running

    int speed = 6;
    // Enter the main event loop
    while (running) {
        // Process events in the queue, it will store the value at each updation to the x, y coordinates of circle
        while (SDL_PollEvent(&event)) {
            // Check if the event is a quit event (e.g., window close)
            if (event.type == SDL_QUIT) { // by pressing cross (X) to exit window
                running = 0;  
            }
            // logic of mouse event handle
            if (event.type == SDL_MOUSEMOTION && event.motion.state != 0)
            {
                circle.x = event.motion.x;
                circle.y = event.motion.y;
                generate_rays(circle, rays);
            }
        }
        // Additional rendering and logic can be performed here

        // we need to erase the circle from the frame before, with surface color, so that it looks the circle is moving
        SDL_FillRect(surface, &erase_rect, COLOR_BLACK);
        FillRays(surface, rays, RAY_COLOR, shadow_circle);
        FillCircle(surface, shadow_circle, COLOR_WHITE); // shadow circle
        FillCircle(surface, circle, COLOR_WHITE); 


        //BasicAntialiasing(surface);
        // calculation about how the y-coordinate of the object should behave
        // when crossing with the speed and it's change in position w.r.t speed
        shadow_circle.y += speed;
        if(shadow_circle.y - shadow_circle.r < 0) {
            speed = -speed;
        } 

        if(shadow_circle.y + shadow_circle.r > HEIGHT) {
            speed = -speed;
        }

        SDL_UpdateWindowSurface(window); // changes to the surface will update her
        SDL_Delay(16);  // Delay to control frame rate; ~16ms for ~60 FPS
    }

    // Destroy the created window to free resources
    SDL_DestroyWindow(window);
    SDL_Quit(); // Clean up all initialized SDL subsystems


    return 0;
}
