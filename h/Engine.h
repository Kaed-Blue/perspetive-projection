#pragma once
#include <SDL3/SDL.h>

class Renderer;
class Camera;

class Engine
{
private:
  Renderer* Erenderer;
  Camera* Ecamera;

public:
  SDL_Window* window;
  SDL_Renderer* sdlRenderer;

  float frame = 0;

#define HEIGHT 700
#define WIDTH 700

  float GetAspectRatio(SDL_Window* window);

  bool Initialize();

  void run();

  void clean();
};