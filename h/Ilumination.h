#pragma once
#include "types.h"
#include <SDL3/SDL.h>

class Ilumination
{
private:
  vec3d lightDir = { 0, 0, -1 };

public:
  vec3d GetLightRay();

  void SetLightDir(vec3d lightDir);

  SDL_FColor ShadowValue(vec3d &normal);
};