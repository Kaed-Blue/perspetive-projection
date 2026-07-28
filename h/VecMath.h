#pragma once
#include "types.h"

class VecMath
{
public:
  vec3d MultiplyMatrixVector(const vec3d& i, const mat4x4& m);

  mat4x4 MultiplyMatrices(const mat4x4& m1, const mat4x4& m2);

  vec3d GetNormal(triangle& tri);

  void Normalize(vec3d& vec);

  float DotProduct(vec3d& v1, vec3d& v2);

  vec3d SubtractVector(vec3d& v1, vec3d& v2);

  void RotationZ(mat4x4& mat, float theta);

  void RotationX(mat4x4& mat, float theta);
};
