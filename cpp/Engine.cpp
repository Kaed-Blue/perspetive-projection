#include "Engine.h"
#include "Renderer.h"
#include "Camera.h"
#include "Mesh.h"
#include "VecMath.h"
#include <array>

float
Engine::GetAspectRatio(SDL_Window* window)
{
  int w, h;
  SDL_GetWindowSize(window, &w, &h);
  return static_cast<float>(w) / static_cast<float>(h);
}

bool
Engine::Initialize()
{
  (SDL_INIT_VIDEO);
  window = SDL_CreateWindow("window", WIDTH, HEIGHT, SDL_WINDOW_OPENGL);
  sdlRenderer = SDL_CreateRenderer(window, nullptr);

  Erenderer = new Renderer(sdlRenderer); // TODO: use the modern version (self delete)
  Ecamera = new Camera();

  if (!window) {
    return false;
  }
  return true;
}

void
Engine::run()
{
  // Load Objects
  Mesh cube;
  // cube.CreateCube();
  cube.LoadFromObj("../Obj/utah_teapot.obj");

  const float DEG2RAD = M_PI / 180.0f;
  Uint64 lastCounter = SDL_GetPerformanceCounter();

  bool running = true;
  while (running) {

    // Get elapsed time since last frame
    Uint64 currentCounter = SDL_GetPerformanceCounter(); // TODO: go find how this works
    float elapsedTime = (float)(currentCounter - lastCounter) / SDL_GetPerformanceFrequency();
    lastCounter = currentCounter;

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        running = false;
      }
    }
    SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 0, 255);
    SDL_RenderClear(sdlRenderer);
    SDL_SetRenderDrawColor(sdlRenderer, 255, 255, 255, 255);

    // Set universal speed
    float dm = elapsedTime * 10;

    // Camera movement
    const bool *keyStates = SDL_GetKeyboardState(NULL);
    if (keyStates[SDL_SCANCODE_D])
    {
      Ecamera->MoveCameraLocal({-dm, 0, 0});
    }

    if (keyStates[SDL_SCANCODE_A])
    {
      Ecamera->MoveCameraLocal({dm, 0, 0});
    }

    if (keyStates[SDL_SCANCODE_W])
    {
      Ecamera->MoveCameraLocal({0, dm, 0});
    }

    if (keyStates[SDL_SCANCODE_S])
    {
      Ecamera->MoveCameraLocal({0, -dm, 0});
    }

    if (keyStates[SDL_SCANCODE_C])
    {
      Ecamera->MoveCameraLocal({0, 0, dm * 5});
    }

    if (keyStates[SDL_SCANCODE_X])
    {
      Ecamera->MoveCameraLocal({0, 0, -dm * 5});
    }

    // Camera diraction
    if (keyStates[SDL_SCANCODE_J])
    {
      Ecamera->ChangeYawBy(dm);
    }

    if (keyStates[SDL_SCANCODE_L])
    {
      Ecamera->ChangeYawBy(-(dm));
    }

    static vec3d angel;
    angel.x += 40 * elapsedTime * DEG2RAD;
    angel.y += 20 * elapsedTime * DEG2RAD;
    angel.z += 10 * elapsedTime * DEG2RAD;

    Erenderer->DrawMesh(cube, {0, 0, 0}, *Ecamera, {0, 0, 150});

    SDL_RenderPresent(sdlRenderer);
  }
  clean();
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void
Engine::clean()
{
  delete Erenderer;
  delete Ecamera;
}
