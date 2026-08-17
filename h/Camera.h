#pragma once

#include "types.h"

class Camera
{
private:
  vec3d position = {0.0f, 0.0f, 0.0f};
  vec3d forward = {0.0f, 0.0f, 1.0f};
  float yaw = 0;
  float pitch = 0;
  float roll = 0;
  mat4x4 viewMatrix;

public:
  vec3d GetCameraPos() const;
  vec3d GetForward() const;
  float GetYaw() const;
  mat4x4 GetViewMatrix() const;

  vec3d GetRight() const;
  vec3d GetUp() const;

  void MoveCameraWorld(const vec3d &move);
  void TeleportCameraWorld(const vec3d &location);
  void MoveCameraLocal(const vec3d &move);
  void TeleportCameraLocal(const vec3d &location);

  void ChangeDiraction(const vec3d &theta);

  void UpdateViewMatrix();
};
