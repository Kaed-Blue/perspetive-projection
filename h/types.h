#pragma once
#include <SDL3/SDL.h>

struct vec3d
{
  float x, y, z;
  SDL_FColor color;
};

struct triangle
{
  vec3d p[3];
};

struct mat4x4
{
  float m[4][4] = { 0 };
};
