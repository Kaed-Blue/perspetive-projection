#include "Engine.h"

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

  Erenderer = new Renderer(sdlRenderer);
  Ecamera = new Camera();

  if (!window) {
    return false;
  }
  return true;
}

void
Engine::run()
{
  bool running = true;
  while (running) {
    SDL_Delay(20);
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        running = false;
      }
    }

    SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 0, 255);
    SDL_RenderClear(sdlRenderer);
    SDL_SetRenderDrawColor(sdlRenderer, 255, 255, 255, 255);

    frame += 1;

    Mesh cube;
    cube.CreateCube();

    Erenderer->DrawMesh(cube.tris, frame, Ecamera->GetCameraPos());
    SDL_RenderPresent(sdlRenderer);
  }
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void
Engine::clean()
{
  delete Erenderer;
}
