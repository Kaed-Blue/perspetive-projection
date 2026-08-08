#include "VecMath.h"
#include <math.h>

vec3d
VecMath::MultiplyMatrixVector(const vec3d& i, const mat4x4& m)
{
  vec3d o;
  o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
  o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
  o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];
  float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];

  if (w != 0.0f) {
    o.x /= w;
    o.y /= w;
    o.z /= w;
  }
  return o;
}

mat4x4
VecMath::MultiplyMatrices(const mat4x4& m1, const mat4x4& m2)
{
  mat4x4 res;

  for (int row = 0; row < 4; row++) {
    for (int col = 0; col < 4; col++) {

      res.m[row][col] =
        m1.m[row][0] * m2.m[0][col] + m1.m[row][1] * m2.m[1][col] +
        m1.m[row][2] * m2.m[2][col] + m1.m[row][3] * m2.m[3][col];
    }
  }
  return res;
}

mat4x4
VecMath::TranslateMatrix(const float &x, const float &y, const float &z)
{
  mat4x4 mat = MakeIdentity();
  mat.m[3][0] = x;
  mat.m[3][1] = y;
  mat.m[3][2] = z;
  return mat;
}

mat4x4
VecMath::MakeIdentity()
{
  mat4x4 mat;
  mat.m[0][0] = 1;
  mat.m[1][1] = 1;
  mat.m[2][2] = 1;
  mat.m[3][3] = 1;

  return mat;
}

vec3d VecMath::GetVectorNormal(const vec3d &vec1, const vec3d &vec2) // gives normalized cross product
{
  vec3d normal;

  normal.x = (vec1.y * vec2.z) - (vec2.y * vec1.z);
  normal.y = (vec1.z * vec2.x) - (vec2.z * vec1.x);
  normal.z = (vec1.x * vec2.y) - (vec2.x * vec1.y);

  Normalize(normal);

  return normal;
}

void
VecMath::Normalize(vec3d& vec)
{
  float magnitude = sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
  if (magnitude != 0)
  {
    vec.x /= magnitude;
    vec.y /= magnitude;
    vec.z /= magnitude;
  }
}

float VecMath::DotProduct(const vec3d &v1, const vec3d &v2)
{
  return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

vec3d VecMath::SubtractVector(const vec3d &v1, const vec3d &v2)
{
  vec3d res;
  res.x = v1.x - v2.x;
  res.y = v1.y - v2.y;
  res.z = v1.z - v2.z;
  return res;
}

vec3d VecMath::AddVector(const vec3d &v1, const vec3d &v2)
{
  vec3d res;
  res.x = v1.x + v2.x;
  res.y = v1.y + v2.y;
  res.z = v1.z + v2.z;
  return res;
}

vec3d VecMath::ScaleVector(const vec3d &v1, const vec3d &v2)
{
  return {v1.x * v2.x, v1.y * v2.y, v1.z * v2.z};
}

vec3d VecMath::ScaleVector(const vec3d &v, const float x)
{
  return {v.x * x, v.y * x, v.z * x};
}

void VecMath::RotationX(mat4x4 &mat, const float angel)
{
  mat.m[0][0] = 1;
  mat.m[1][1] = cosf(angel);
  mat.m[1][2] = sinf(angel);
  mat.m[2][1] = -sinf(angel);
  mat.m[2][2] = cosf(angel);
  mat.m[3][3] = 1;
}

void VecMath::RotationY(mat4x4 &mat, const float angel)
{
  mat.m[0][0] = cosf(angel);
  mat.m[0][2] = sinf(angel);
  mat.m[1][1] = 1;
  mat.m[2][0] = -sinf(angel);
  mat.m[2][2] = cosf(angel);
  mat.m[3][3] = 1;
}

void VecMath::RotationZ(mat4x4 &mat, const float angel)
{
  mat.m[0][0] = cosf(angel);
  mat.m[0][1] = sinf(angel);
  mat.m[1][0] = -sinf(angel);
  mat.m[1][1] = cosf(angel);
  mat.m[2][2] = 1;
  mat.m[3][3] = 1;
}
