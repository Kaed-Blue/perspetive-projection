#pragma once
#include "types.h"
#include <vector>

class Clipper
{
private:
  vec3d plainNormals[6];
  vec3d plainPoints[6];

public:
  Clipper(float vFovRad, float aspectRatio, float nearPlain, float farPlain);

  static int ClipAgainstPlain(const vec3d &pointOnPlain, const vec3d &normal, const triangle &inTri, triangle &outTri1, triangle &outTri2);

  static bool ClipAgainstPlain(const vec3d &pointOnPlain, const vec3d &normal, const vec3d &lineStart, vec3d &lineEnd, vec3d &p1Clipped, vec3d &p2clipped);

  std::vector<triangle> ClipSpace(const triangle &inTri);

  void UpdatePlainNormals(const float FOV, const float AspectRatio);
};