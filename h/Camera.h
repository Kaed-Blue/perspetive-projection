#pragma once

#include "types.h"

class Camera
{
private:
  vec3d position = {0.0f, 0.0f, 0.0f};
  vec3d forward = {0.0f, 0.0f, 1.0f};
  float yaw = 0;

public:
  vec3d GetCameraPos() const;
  vec3d GetForward() const;
  float GetYaw() const;
  
  vec3d GetRight() const;
  vec3d GetUp() const;

  void MoveCameraWorld(const float x, const float y, const float z);
  void TeleportCamera(const float x, const float y, const float z);
  void ChangeYawBy(const float x);

  mat4x4 MakeViewMatrix() const;
};
