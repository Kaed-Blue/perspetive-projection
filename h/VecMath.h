#pragma once
#include "types.h"

class VecMath
{
public:
  static vec3d MultiplyMatrixVector(const vec3d &i, const mat4x4 &m);

  static mat4x4 MultiplyMatrices(const mat4x4 &m1, const mat4x4 &m2);

  static mat4x4 TranslateMatrix(const float &x, const float &y, const float &z);

  static mat4x4 MakeIdentity();

  static vec3d GetVectorNormal(triangle &tri);

  static void Normalize(vec3d &vec);

  static float DotProduct(vec3d &v1, vec3d &v2);

  static vec3d SubtractVector(vec3d &v1, vec3d &v2);

  static vec3d AddVector(vec3d &v1, vec3d &v2);

  static vec3d ScaleVector(vec3d &v1, vec3d &v2);

  static void RotationX(mat4x4 &mat, float theta, float speed);

  static void RotationY(mat4x4 &mat, float theta, float speed);

  static void RotationZ(mat4x4 &mat, float theta, float speed);
};
