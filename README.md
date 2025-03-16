# Ray Tracing in C

In 3D computer graphics, ray tracing is a technique for modeling light transport for use in a wide variety of rendering algorithms for generating digital images.

It's a computer graphics technique that mimics how light works in real life. It traces the path of rays of light as they travel, bounce, reflect, or pass through objects, capturing details like shadows, reflections, and refractions. Imagine shiny floors, crystal-clear glass, or water ripples—ray tracing makes them all look jaw-droppingly realistic.

## Documentation

This project uses the SDL2 library for graphic rendering. Below is a brief explanation of the key components of the code in

### Key Components

1. **Initialization**
    - The SDL2 video subsystem is initialized. If initialization fails, an error message is printed and the program exits.

2. **Window Creation**
    - An SDL window is created with the title "Ray Tracing", centered on the screen with a width of 800 pixels and a height of 600 pixels. If window creation fails, an error message is printed, SDL subsystems are cleaned up, and the program exits.

3. **Event Handling**
    - An SDL event variable is declared to handle events. A flag (`running`) is used to control the main loop. The main event loop processes events in the queue and checks if the event is a quit event (e.g., window close). If a quit event is detected, the `running` flag is set to 0 to exit the loop.

4. **Frame Rate Control**
    - A delay is introduced to control the frame rate, approximately 16ms for ~60 FPS.

5. **Cleanup**
    - The created window is destroyed to free resources, and all initialized SDL subsystems are cleaned up before the program exits.

### How to Compile and Run

1. **Install SDL2 Library**
    - On macOS, you can install SDL2 using Homebrew:

      ```sh
      brew install sdl2
      ```

2. **Compile the Code**

    ```sh
    gcc -o raytracing raytracing.c -I/opt/homebrew/include/SDL2 -D_THREAD_SAFE -L/opt/homebrew/lib -lSDL2
    ```

3. **Run the Executable**

    ```sh
    ./raytracing
    ```

This README provides an overview of the raytracing.c file, explaining the key components of the code and how to compile and run the program.

### Creation of a cirlce In SDL 2
>
> **NOTE**: SDL 2 does not have a built-in function to draw a circle. However, you can create a circle by drawing multiple points along the circumference of the circle.

```c
void FillCircle(SDL_Surface *s, Circle circle, Uint32 color) {};
```

- We can create a circle by treating each rectangle as a pixel, here we can use the equation of a circle to draw the circle.

- By calculating, distance of radius, x and y corrdinate of the circle, and loop over it can give us the circle.

- x-radius, x+radius, y-radius, y+radius are the boundries of the circle and looping over it can give us the circle.

- we will take the distance square, and radius square and check if the distance square is less than the radius square, then circle will be drawn.

### Movement of the Circle

We can move the circle by changing the x and y coordinate of the circle.

#### Updating Circle Movements with SDL Events

To update the circle movements, we handle SDL events in the main event loop. Specifically, we handle SDL_MOUSEMOTION events to update the circle's position based on the mouse's coordinates.

1. **Event Handling for Circle Movement**

- In the main event loop, we check for `SDL_MOUSEMOTION` events.
- If a `SDL_MOUSEMOTION` event is detected and the mouse button is pressed, we update the circle's x and y coordinates to the mouse's current position.

### `BasicAntialiasing(SDL_Surface *surface)`

Applies a basic box blur effect to smooth edges in the rendered image.

- **Parameters:**
  - `surface`: SDL surface to apply anti-aliasing to
- **Process:**
  - Uses a 3x3 kernel to average neighboring pixels
  - Handles surface locking for thread safety
  - Processes RGB channels separately
  - Creates temporary buffer to store original pixels
  - Applies averaging calculation for each pixel

### `generate_rays(Circle circle, Ray rays[RAYS_NUMBER])`

Creates an array of rays emanating from a circle's center point.

- **Parameters:**
  - `circle`: Source circle from which rays originate
  - `rays`: Array to store generated rays
- **Process:**
  - Distributes rays evenly in 360 degrees
  - Each ray stores starting position and angle
  - Uses circle center as ray origin point

### `FillRays(SDL_Surface *surface, Ray rays[RAYS_NUMBER], Uint32 color, Circle Object)`

Renders rays and handles ray-circle intersection calculations.

- **Parameters:**
  - `surface`: SDL surface to draw on
  - `rays`: Array of rays to render
  - `color`: Color of the rays
  - `Object`: Circle to check for intersections
- **Process:**
  - Iterates through each ray
  - Incrementally draws ray paths
  - Checks for collisions with object
  - Stops ray rendering at screen boundaries or object intersections
  - Uses distance formula for collision detection

### `FillCircle(SDL_Surface *s, Circle circle, Uint32 color)`

Draws a filled circle on the SDL surface.

- **Parameters:**
  - `s`: SDL surface to draw on
  - `circle`: Circle structure containing position and radius
  - `color`: Color to fill the circle with
- **Process:**
  - Uses distance formula to determine which pixels fall within circle
  - Iterates through rectangular boundary of circle
  - Fills pixels that fall within radius
  - Creates solid circle appearance

### `main()`

Main program loop and initialization.

- **Process:**
  - Initializes SDL video subsystem
  - Creates window and gets surface
  - Sets up initial circle and shadow circle objects
  - Handles event loop for:
    - Window closing
    - Mouse movement
    - Circle updates
  - Manages animation timing
  - Implements bouncing movement for shadow circle
  - Cleans up resources on exit

  ## Ray Tracing Functions Documentation

    | Function Name | Parameters | Return Type | Description |
    |--------------|------------|-------------|-------------|
    | `BasicAntialiasing` | `SDL_Surface *surface` | `void` | Applies a box blur effect to smooth edges in the rendered image. Works with RGB888 or ARGB8888 formats. |
    | `generate_rays` | `Circle circle`, `Ray rays[RAYS_NUMBER]` | `void` | Creates an array of rays emanating from a circle's center point at evenly distributed angles. |
    | `FillRays` | `SDL_Surface *surface`, `Ray rays[RAYS_NUMBER]`, `Uint32 color`, `Circle Object` | `void` | Renders rays on the surface and handles ray-circle intersection detection. |
    | `FillCircle` | `SDL_Surface *s`, `Circle circle`, `Uint32 color` | `void` | Draws a filled circle on the surface using the specified color. |

  ## Structures

  ### Circle

    | Member | Type | Description |
    |--------|------|-------------|
    | `x` | `int` | X-coordinate of circle's center |
    | `y` | `int` | Y-coordinate of circle's center |
    | `r` | `int` | Radius of the circle |

  ### Ray

    | Member | Type | Description |
    |--------|------|-------------|
    | `x_start` | `double` | Starting X-coordinate of the ray |
    | `y_start` | `double` | Starting Y-coordinate of the ray |
    | `angle` | `double` | Angle of the ray in radians |

  ## Constants

    | Constant | Value | Description |
    |----------|-------|-------------|
    | `WIDTH` | 1200 | Window width in pixels |
    | `HEIGHT` | 600 | Window height in pixels |
    | `RAYS_NUMBER` | 500 | Number of rays to generate |
    | `RAY_THICKNESS` | 2 | Thickness of rendered rays |

  ## 2D Raytracing Project Documentation

  ## Overview

    This project implements a basic 2D raytracing system using C and SDL2. It demonstrates fundamental concepts of ray casting and collision detection in a real-time interactive environment.

  ## Core Components

  ### Data Structures

    1. `Circle`: Represents circular objects with position (x, y) and radius (r)
    2. `Ray`: Defines rays with starting position and angle of propagation

  ### Key Features

  - Real-time ray casting from a movable source
  - Dynamic shadow casting
  - Interactive mouse control
  - Collision detection between rays and circular objects
  - Basic anti-aliasing implementation (optional feature)

  ### Technical Implementation

  #### Ray Generation

  - Rays are generated in a 360-degree pattern around a source point
  - Total of 500 rays (configurable via RAYS_NUMBER)
  - Each ray is calculated using trigonometric functions for direction

  #### Rendering System

  - Uses SDL2 for window management and rendering
  - Resolution: 1200x600 pixels
  - Implements real-time surface updates
  - Frame rate control (~60 FPS)

  #### Collision Detection

  - Uses circle-ray intersection mathematics
  - Distance-based collision calculation
  - Efficient radius-squared comparisons

  #### Interactive Elements

  - Mouse-controlled light source
  - Bouncing shadow object
  - Real-time ray updates

  ## Performance Considerations

  - Optimized ray casting using step-based increments
  - Efficient memory management for surface operations
  - Basic anti-aliasing available but optional due to performance impact

  ## Usage

    The program creates a window where users can:
    1. Move the light source (white circle) using mouse drag
    2. Observe real-time ray casting and shadow effects
    3. Watch the interaction with a bouncing circular obstacle

  ## Technical Requirements

  - SDL2 library
  - C compiler with math.h support
  - Minimum screen resolution support: 1200x600

  ## Limitations

  - Limited to circular objects for collision
  - 2D environment only
  - Fixed resolution
  - Basic anti-aliasing might impact performance

  ## Future Improvements

  - Multiple light sources support
  - Different object shapes
  - Optimized anti-aliasing
  - Color and intensity gradients
  - Dynamic resolution scaling

## Project Summary
>
> This project demonstrates a 2D ray tracing implementation in C using SDL2. It features real-time ray casting, dynamic shadow generation, and interactive mouse control. The system generates 500 rays in a 360-degree pattern from a movable light source, calculating collisions with circular objects. Key features include efficient collision detection, basic anti-aliasing, and a 60 FPS rendering system. The implementation showcases fundamental graphics programming concepts through a practical, interactive visualization running at 1200x600 resolution.
