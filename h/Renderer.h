#pragma once
#include "Ilumination.h"
#include "types.h"
#include <SDL3/SDL.h>
#include <math.h>
#include <vector>

class Mesh;
class Camera;

class Renderer
{
private:
  float Near = 0.1f;
  float Far = 1000.0f;
  float Fov = 90.0f;
  float FovRad = 1.0f / tanf(Fov * 0.5f / 180.0f * 3.14159f);
  float aspectRatio;
  SDL_Renderer *sdlrenderer;
  Ilumination ilumination;
  mat4x4 matProj;
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

  void DrawMesh(Mesh mesh, vec3d angel, const Camera &camera, vec3d objPos);
};