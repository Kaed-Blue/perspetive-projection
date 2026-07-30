#pragma once
#include "types.h"
#include <SDL3/SDL.h>

class Ilumination
{
private:
  vec3d lightRay = { 0, 0, -1 };

public:
  vec3d GetLightRay();

  SDL_FColor ShadowValue(vec3d &normal);
};