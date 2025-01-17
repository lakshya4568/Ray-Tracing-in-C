#include <stdio.h>
#include <math.h>
#include </opt/homebrew/include/sdl2/SDL.h>


#define WIDTH 1000 
#define HEIGHT 1000
#define RAYS_NUMBER 200
#define COLOR_WHITE 0xffffffff
#define COLOR_BLACK 0x00000000
#define RAY_COLOR 0xde2508
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
    double angle;
    /* Origin coordinate of ray to end of ray*/
    double x_start, y_start; 
} Ray;

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
        Ray ray = {angle, circle.x, circle.y};
        rays[i] = ray;
        printf("Angle: %f\n", angle);
    }
}

void FillRays(SDL_Surface *surface, Ray rays[RAYS_NUMBER], Uint32 color, Circle Object) {

    double radius_squared = pow(Object.r, 2);

    for (int i = 0; i < RAYS_NUMBER; i++)
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
        SDL_WINDOW_SHOWN        // Window is visible upon creation
    );

    // retrieve the surface in case we need to draw anything to the window (a sheet)
    SDL_Surface *surface = SDL_GetWindowSurface(window);

    /* creation of a rectangle in sdl 
     SDL_Rect rect = (SDL_Rect){200, 200, 200, 200};
     rect.h, rect.w, rect.x, rect.y = 200;
     SDL_FillRect(surface, &rect, COLOR_WHITE);
    */

    Circle circle = {200, 200, 80};
    SDL_Rect erase_rect = (SDL_Rect){0, 0, WIDTH, HEIGHT};

    // shadow circle
    Circle shadow_circle = {650, 300, 120};
    Ray rays[RAYS_NUMBER];


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
        FillCircle(surface, shadow_circle, COLOR_WHITE); // shadow circle
        FillCircle(surface, circle, COLOR_WHITE);
        FillRays(surface, rays, RAY_COLOR, shadow_circle);
        SDL_UpdateWindowSurface(window); // changes to the surface will update here
        SDL_Delay(16);  // Delay to control frame rate; ~16ms for ~60 FPS
    }

    // Destroy the created window to free resources
    SDL_DestroyWindow(window);
    SDL_Quit(); // Clean up all initialized SDL subsystems


    return 0;
}
