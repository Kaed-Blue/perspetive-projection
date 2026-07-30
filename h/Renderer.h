#pragma once
#include "Ilumination.h"
#include "types.h"
#include <SDL3/SDL.h>
#include <math.h>
#include <vector>

class Renderer
{
private:
  float fNear = 0.1f;
  float fFar = 1000.0f;
  float fFov = 90.0f;
  float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);
  float aspectRatio;
  SDL_Renderer *sdlrenderer;
  Ilumination Rilumination;
  mat4x4 matproj;
  mat4x4 matRotX, matRotY, matRotZ;

public:
  Renderer(SDL_Renderer* sdlrenderer);

  void DrawTriangle(SDL_Renderer* sdlrenderer,
                    int a_x,
                    int a_y,
                    int b_x,
                    int b_y,
                    int c_x,
                    int c_y);

  void DrawMesh(std::vector<triangle> tris, int frame, vec3d cameraPos);
};