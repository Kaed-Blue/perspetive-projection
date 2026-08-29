#include "Clipper.h"
#include "VecMath.h"
#include <math.h>
#include <vector>

Clipper::Clipper(float vFOV, float aspectRatio, float nearPlain, float farPlain)
{
  this->UpdatePlainNormals(vFOV, aspectRatio);
  this->plainNormals[4] = {0.0f, 0.0f, 1.0f};
  this->plainNormals[5] = {0.0f, 0.0f, -1.0f};

  this->plainPoints[0] = {0.0f, 0.0f, 0.0f};
  this->plainPoints[1] = {0.0f, 0.0f, 0.0f};
  this->plainPoints[2] = {0.0f, 0.0f, 0.0f};
  this->plainPoints[3] = {0.0f, 0.0f, 0.0f};
  this->plainPoints[4] = {0.0f, 0.0f, nearPlain};
  this->plainPoints[5] = {0.0f, 0.0f, farPlain};
}

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
    if (distanceToPlain <= pdistance) // plain normal is pointing inside
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
    throw;
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

std::vector<triangle>
Clipper::ClipSpace(const triangle &inTri)
{
  std::vector<triangle> triangles, clipped;
  triangles.reserve(16);
  clipped.reserve(16);
  triangles.push_back(inTri);

  triangle temp1, temp2;
  for (int n = 0; n < 6; n++)
  {
    for (const triangle &tri : triangles)
    {
      int triCount = this->ClipAgainstPlain(this->plainPoints[n], this->plainNormals[n], tri, temp1, temp2);
      if (triCount == 0)
        continue;

      if (triCount == 1)
        clipped.push_back(temp1);

      else if (triCount == 2)
      {
        clipped.push_back(temp1);
        clipped.push_back(temp2);
      }
    }
    triangles = std::move(clipped);

    if (triangles.empty())
      break;
  }

  return triangles;
}

void Clipper::UpdatePlainNormals(const float vFOV, const float aspectRatio)
{
  float HalfvFov = (vFOV)*M_PI / 360.0f;
  float HalfhFov = atanf(tanf(HalfvFov) * aspectRatio);

  // Top plain normal
  this->plainNormals[0] = {0.0f, -sinf(HalfvFov), cosf(HalfvFov)};

  // Right plain normal
  this->plainNormals[1] = {cosf(HalfhFov), 0.0f, sinf(HalfhFov)};

  // Bottom plain normal
  this->plainNormals[2] = {0.0f, sinf(HalfvFov), cosf(HalfvFov)};

  // Left plain normal
  this->plainNormals[3] = {-cosf(HalfhFov), 0.0f, sinf(HalfhFov)};
}