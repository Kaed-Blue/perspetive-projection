#pragma once
#include <SDL3/SDL.h>

struct vec3d
{
  float x = 0, y = 0, z = 0;
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
// TODO: make cout able to show vec3d
