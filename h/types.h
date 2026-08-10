#pragma once
#include <SDL3/SDL.h>

struct vec3d
{
  float x = 0, y = 0, z = 0;
  SDL_FColor color;

  vec3d operator+(const vec3d &other) const;
  vec3d &operator+=(const vec3d &other);
  vec3d operator-(const vec3d &other) const;
  vec3d &operator-=(const vec3d &other);
  vec3d operator*(const vec3d &other) const;
  vec3d operator*(const float other) const;
};

struct triangle
{
  vec3d p[3];
};

struct mat4x4
{
  float m[4][4] = { 0 };

  mat4x4
  operator*(const mat4x4 &other) const;
};
// TODO: make cout able to show vec3d
