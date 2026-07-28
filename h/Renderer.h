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
  SDL_Renderer* sdlrenderer;
  Ilumination Rilumination;
  mat4x4 matproj;
  mat4x4 matRotZ, matRotX;

public:
  Renderer(SDL_Renderer* sdlrenderer);

  vec3d MultiplyMatrixVector(const vec3d& i, const mat4x4& m);

  mat4x4 MultiplyMatrices(const mat4x4& m1, const mat4x4& m2);

  vec3d GetNormal(triangle& tri);

  void Normalize(vec3d& vec);

  float DotProduct(vec3d& v1, vec3d& v2);

  vec3d SubtractVector(vec3d& v1, vec3d& v2);

  void RotationZ(float theta);

  void RotationX(float theta);

  void DrawTriangle(SDL_Renderer* sdlrenderer,
                    int a_x,
                    int a_y,
                    int b_x,
                    int b_y,
                    int c_x,
                    int c_y);

  void DrawMesh(std::vector<triangle> tris, int frame, vec3d cameraPos);
};