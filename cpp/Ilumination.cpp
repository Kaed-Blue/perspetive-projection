#include "Ilumination.h"
#include "VecMath.h"

vec3d Ilumination::GetLightRay() const
{
  return this->lightDir;
}

void Ilumination::SetLightDir(const vec3d &lightDir)
{
  this->lightDir = lightDir;
}

SDL_FColor
Ilumination::ShadowValue(const vec3d &normal) const
{
  // beware! lightDir may not be normalized
  float LumDP = VecMath::DotProduct(normal, this->lightDir);
  int ilum = LumDP * -10;
  switch (ilum)
  {
  case 0:
    return {0.05, 0.05, 0.05, 1};

  case 1:
    return {0.111, 0.111, 0.111, 1};

  case 2:
    return {0.222, 0.222, 0.222, 1};

  case 3:
    return {0.333, 0.333, 0.333, 1};

  case 4:
    return {0.444, 0.444, 0.444, 1};

  case 5:
    return {0.555, 0.555, 0.555, 1};

  case 6:
    return {0.666, 0.666, 0.666, 1};

  case 7:
    return {0.777, 0.777, 0.777, 1};

  case 8:
    return {0.888, 0.888, 0.888, 1};

  case 9:
    return {0.999, 0.999, 0.999, 1};

  case 10:
    return {1, 1, 1, 1};

  default:
    return {0, 0, 0, 1};
  }
}