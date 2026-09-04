#include "Clipper.h"
#include "VecMath.h"
#include <math.h>
#include <array>
#include "profiler.h"

Clipper::Clipper(float vFOV, float aspectRatio, float nearPlain, float farPlain)
{
  using namespace VecMath;

  this->UpdatePlainNormals(vFOV, aspectRatio);
  this->plain[4].normal = {0.0f, 0.0f, -1.0f};
  this->plain[5].normal = {0.0f, 0.0f, 1.0f};

  this->plain[0].pointOnPlain = {0.0f, 0.0f, 0.0f};
  this->plain[1].pointOnPlain = {0.0f, 0.0f, 0.0f};
  this->plain[2].pointOnPlain = {0.0f, 0.0f, 0.0f};
  this->plain[3].pointOnPlain = {0.0f, 0.0f, 0.0f};
  this->plain[4].pointOnPlain = {0.0f, 0.0f, farPlain};
  this->plain[5].pointOnPlain = {0.0f, 0.0f, nearPlain};

  this->plain[0].offset = -DotProduct(plain[0].normal, {0.0f, 0.0f, 0.0f});
  this->plain[1].offset = -DotProduct(plain[1].normal, {0.0f, 0.0f, 0.0f});
  this->plain[2].offset = -DotProduct(plain[2].normal, {0.0f, 0.0f, 0.0f});
  this->plain[3].offset = -DotProduct(plain[3].normal, {0.0f, 0.0f, 0.0f});
  this->plain[4].offset = -DotProduct(plain[4].normal, {0.0f, 0.0f, farPlain});
  this->plain[5].offset = -DotProduct(plain[5].normal, {0.0f, 0.0f, nearPlain});
}

int Clipper::ClipAgainstPlain(const Plain &plain, const triangle &inTri, triangle &outTri1, triangle &outTri2) // For a triangle
{
  vec3d inside[3];
  int insideCount = 0;
  vec3d outside[3];
  int outsideCount = 0;

  for (int i = 0; i < 3; i++)
  {
    float pdistance = VecMath::DotProduct(plain.normal, inTri.p[i]);
    if (plain.offset >= -pdistance) // plain normal is pointing inside
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
    VecMath::PlainIntersect(plain.pointOnPlain, plain.normal, inside[0], outside[0], intersection[0]);
    VecMath::PlainIntersect(plain.pointOnPlain, plain.normal, inside[0], outside[1], intersection[1]);
    outTri1 = {inside[0], intersection[0], intersection[1]};
    return 1;

  case 2:
    VecMath::PlainIntersect(plain.pointOnPlain, plain.normal, inside[0], outside[0], intersection[0]);
    VecMath::PlainIntersect(plain.pointOnPlain, plain.normal, inside[1], outside[0], intersection[1]);

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

ClipResult
Clipper::ClipSpace(const triangle &inTri) // FIXME: performace issue
{
  std::array<triangle, 8> triangles; // you should be able to only use one array
  std::array<triangle, 8> clipped;

  triangles[0] = inTri;
  int triangleCount = 1;

  triangle temp1, temp2;
  for (int n = 0; n < 6; n++)
  {
    int clippedCount = 0;

    for (int i = 0; i < triangleCount; i++)
    {
      switch (this->ClipAgainstPlain(this->plain[n], triangles[i], temp1, temp2))
      {
      case 0:
        continue;

      case 1:
        clipped[clippedCount++] = temp1;
        break;

      case 2:
        clipped[clippedCount++] = temp1;
        clipped[clippedCount++] = temp2;
        break;
      }
    }

    std::swap(triangles, clipped);
    triangleCount = clippedCount;

    if (triangleCount == 0)
      break;
  }

  return {triangles, triangleCount};
}

void Clipper::UpdatePlainNormals(const float vFOV, const float aspectRatio)
{
  float HalfvFov = (vFOV)*M_PI / 360.0f;
  float HalfhFov = atanf(tanf(HalfvFov) * aspectRatio);

  // Top plain normal
  this->plain[0].normal = {0.0f, -sinf(HalfvFov), cosf(HalfvFov)};

  // Right plain normal
  this->plain[1].normal = {cosf(HalfhFov), 0.0f, sinf(HalfhFov)};

  // Bottom plain normal
  this->plain[2].normal = {0.0f, sinf(HalfvFov), cosf(HalfvFov)};

  // Left plain normal
  this->plain[3].normal = {-cosf(HalfhFov), 0.0f, sinf(HalfhFov)};
}