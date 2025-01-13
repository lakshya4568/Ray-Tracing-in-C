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


