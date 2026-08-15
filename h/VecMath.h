#pragma once
#include "types.h"

class VecMath
{
public:

  static vec3d MultiplyMatrixVector(const vec3d &i, const mat4x4 &m);

  static mat4x4 MultiplyMatrices(const mat4x4 &m1, const mat4x4 &m2);

  static mat4x4 TranslateMatrix(const float &x, const float &y, const float &z);

  static mat4x4 MakeIdentity();

  static vec3d CrossProduct(const vec3d &vec1, const vec3d &vec2);

  static vec3d &Normalize(vec3d &vec);
  static vec3d GetNormalized(vec3d vec);

  static float DotProduct(const vec3d &v1, const vec3d &v2);

  static vec3d SubtractVector(const vec3d &v1, const vec3d &v2);
  static vec3d AddVector(const vec3d &v1, const vec3d &v2);
  static vec3d ScaleVector(const vec3d &v1, const vec3d &v2);
  static vec3d ScaleVector(const vec3d &v, const float x);

  static void CoutVec(const vec3d &v);

  static void RotationX(mat4x4 &mat, const float theta);
  static void RotationY(mat4x4 &mat, const float theta);
  static void RotationZ(mat4x4 &mat, const float theta);

  static vec3d PlainIntersect(const vec3d &pointOnPlain, const vec3d &normal, const vec3d &lineStart, const vec3d &lineEnd);
};
