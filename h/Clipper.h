#pragma once
#include "types.h"

class Clipper
{
public:
  static int ClipAgainstPlain(const vec3d &pointOnPlain, const vec3d &normal, const triangle &inTri, triangle &outTri1, triangle &outTri2);

  static bool ClipAgainstPlain(const vec3d &pointOnPlain, const vec3d &normal, const vec3d &lineStart, vec3d &lineEnd, vec3d &p1Clipped, vec3d &p2clipped);
};