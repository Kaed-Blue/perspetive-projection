#pragma once
#include <SDL3/SDL.h>

struct vec3d
{
  float x = 0, y = 0, z = 0;
  SDL_FColor color;

  vec3d operator+(const vec3d& other) const
  {
    return {this->x + other.x, this->y + other.y, this->z + other.z};
  }

  vec3d operator+=(const vec3d& other)
  {
    return {this->x += other.x, this->y += other.y, this->z += other.z};
  }

  vec3d operator-(const vec3d& other) const
  {
    return {this->x - other.x, this->y - other.y, this->z - other.z};
  }

  vec3d operator-=(const vec3d& other)
  {
    return {this->x -= other.x, this->y -= other.y, this->z -= other.z};
  }

  vec3d operator*(const vec3d& other) const
  {
    return {this->x * other.x, this->y * other.y, this->z * other.z};
  }

  vec3d operator*(const float other) const
  {
    return {this->x * other, this->y * other, this->z * other};
  }
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
