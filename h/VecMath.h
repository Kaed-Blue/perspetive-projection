#pragma once
#include "types.h"

namespace VecMath
{

  mat4x4 MultiplyMatrices(const mat4x4 &m1, const mat4x4 &m2);
  mat4x4 TranslateMatrix(const float x, const float y, const float z);
  mat4x4 MakeIdentity();

  vec3d MultiplyMatrixVector(const vec3d &i, const mat4x4 &m);
  vec3d CrossProduct(const vec3d &vec1, const vec3d &vec2);
  vec3d &Normalize(vec3d &vec);
  vec3d GetNormalized(vec3d vec);
  float DotProduct(const vec3d &v1, const vec3d &v2);

  vec3d SubtractVector(const vec3d &v1, const vec3d &v2);
  vec3d AddVector(const vec3d &v1, const vec3d &v2);
  vec3d ScaleVector(const vec3d &v1, const vec3d &v2);
  vec3d ScaleVector(const vec3d &v, const float x);

  void CoutVec(const vec3d &v);

  void RotationX(mat4x4 &mat, const float theta);
  void RotationY(mat4x4 &mat, const float theta);
  void RotationZ(mat4x4 &mat, const float theta);

  bool PlainIntersect(const vec3d &pointOnPlain, const vec3d &normal, const vec3d &lineStart, const vec3d &lineEnd, vec3d &output);
};
