#include "types.h"

// vec3d
vec3d vec3d::operator+(const vec3d &other) const
{
  return {this->x + other.x, this->y + other.y, this->z + other.z};
}

vec3d &vec3d::operator+=(const vec3d &other)
{
  {
    this->x += other.x, this->y += other.y, this->z += other.z;
  }
  return *this;
}

vec3d vec3d::operator-(const vec3d &other) const
{
  return {this->x - other.x, this->y - other.y, this->z - other.z};
}

vec3d &vec3d::operator-=(const vec3d &other)
{
  {
    this->x -= other.x, this->y -= other.y, this->z -= other.z;
  }
  return *this;
}

vec3d vec3d::operator*(const vec3d &other) const
{
  return {this->x * other.x, this->y * other.y, this->z * other.z};
}

vec3d vec3d::operator*(const float other) const
{
  return {this->x * other, this->y * other, this->z * other};
}

// mat4x4
mat4x4 mat4x4::operator*(const mat4x4 &other) const
{
  mat4x4 res;

  for (int row = 0; row < 4; row++)
  {
    for (int col = 0; col < 4; col++)
    {
      res.m[row][col] =
          this->m[row][0] * other.m[0][col] + this->m[row][1] * other.m[1][col] +
          this->m[row][2] * other.m[2][col] + this->m[row][3] * other.m[3][col];
    }
  }
  return res;
}