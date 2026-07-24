#pragma once

#include "types.h"

class Camera
{
private:
  vec3d position = { 0.0f, 0.0f, 0.0f };

public:
  vec3d GetCameraPos();
};
