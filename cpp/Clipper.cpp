#include "Clipper.h"
#include "VecMath.h"

int Clipper::ClipAgainstPlain(const vec3d &pointOnPlain, const vec3d &normal, const triangle &inTri, triangle &outTri1, triangle &outTri2) // For a triangle
{
  vec3d inside[3];
  int insideCount = 0;
  vec3d outside[3];
  int outsideCount = 0;
  float distanceToPlain = VecMath::DotProduct(normal, pointOnPlain); // didn't multi it by -1 to make it dist

  for (int i = 0; i < 3; i++)
  {
    float pdistance = VecMath::DotProduct(normal, inTri.p[i]);
    if (distanceToPlain <= pdistance)
    {
      inside[insideCount++] = inTri.p[i];
    }
    else
    {
      outside[outsideCount++] = inTri.p[i];
    }
  }

  vec3d intersection[3];
  switch (insideCount)
  {
  case 0:
    return 0;

  case 1:
    VecMath::PlainIntersect(pointOnPlain, normal, inside[0], outside[0], intersection[0]);
    VecMath::PlainIntersect(pointOnPlain, normal, inside[0], outside[1], intersection[1]);
    outTri1 = {inside[0], intersection[0], intersection[1]};
    return 1;

  case 2:
    VecMath::PlainIntersect(pointOnPlain, normal, inside[0], outside[0], intersection[0]);
    VecMath::PlainIntersect(pointOnPlain, normal, inside[1], outside[0], intersection[1]);

    outTri1 = {inside[0], inside[1], intersection[0]};
    outTri2 = {inside[1], intersection[0], intersection[1]};
    return 2;

  case 3:
    outTri1 = inTri;
    return 1;

  default:
    return -1;
  }
}

bool Clipper::ClipAgainstPlain(const vec3d &pointOnPlain, const vec3d &normal, const vec3d &lineStart, vec3d &lineEnd, vec3d &p1Clipped, vec3d &p2Clipped) // For a line
{
  float distanceToPlain = VecMath::DotProduct(normal, pointOnPlain);
  float p1distance = VecMath::DotProduct(normal, lineStart);
  float p2distance = VecMath::DotProduct(normal, lineEnd);

  bool p1Inside = distanceToPlain <= p1distance;
  bool p2Inside = distanceToPlain <= p2distance;

  if (p1Inside && p2Inside)
  {
    p1Clipped = lineStart;
    p2Clipped = lineEnd;
    return true;
  }
  else if (!p1Inside && !p2Inside)
  {
    return false;
  }
  else
  {
    vec3d interPoint;
    VecMath::PlainIntersect(pointOnPlain, normal, lineStart, lineEnd, interPoint);

    if (p1Inside)
    {
      p1Clipped = lineStart;
      p2Clipped = interPoint;
      return true;
    }
    else
    {
      p1Clipped = interPoint;
      p2Clipped = lineEnd;
      return true;
    }
  }
}