#include "Camera.h"
#include "VecMath.h"
#include <iostream>

vec3d Camera::GetCameraPos() const
{
  return this->position;
}

vec3d Camera::GetForward() const
{
  return this->forward;
}

float Camera::GetYaw() const
{
  return this->yaw;
}

void Camera::MoveCameraWorld(const float x, const float y, const float z)
{
  this->position.x += x;
  this->position.y += y;
  this->position.z += z;
}

void Camera::TeleportCamera(const float x, const float y, const float z)
{
  this->position.x = x;
  this->position.y = y;
  this->position.z = z;
}

void Camera::ChangeYawBy(const float x)
{
  this->yaw += x;
  vec3d target = {0, 0, 1};
  mat4x4 matCameraRot = VecMath::MakeIdentity();
  VecMath::RotationY(matCameraRot, this->yaw * 3.14159f / 180.0f);
  vec3d lookDir = VecMath::MultiplyMatrixVector(target, matCameraRot);
  std::cout << lookDir.x << "/" << lookDir.y << "/" << lookDir.z << "\n";
  ;
  this->forward = lookDir;
}

vec3d Camera::GetRight() const
{
  vec3d up = {0, 1, 0};
  return VecMath::GetVectorNormal(up, this->forward);
}

vec3d Camera::GetUp() const
{
  vec3d right = GetRight();
  return VecMath::GetVectorNormal(this->forward, right); // maybe normalize for good measure
}

mat4x4
Camera::MakeViewMatrix() const
{
  vec3d right = this->GetRight();
  vec3d newUp = this->GetUp();

  // std::cout << newUp.x << "/" << newUp.y << "/" << newUp.z << "\n";
  // std::cout << right.x << "/" << right.y << "/" << right.z << "\n";

  mat4x4 viewMatrix;
  viewMatrix.m[0][0] = right.x;
  viewMatrix.m[1][0] = right.y;
  viewMatrix.m[2][0] = right.z;
  viewMatrix.m[3][0] = -VecMath::DotProduct(this->position, right);
  viewMatrix.m[0][1] = newUp.x;
  viewMatrix.m[1][1] = newUp.y;
  viewMatrix.m[2][1] = newUp.z;
  viewMatrix.m[3][1] = -VecMath::DotProduct(this->position, newUp);
  viewMatrix.m[0][2] = this->forward.x;
  viewMatrix.m[1][2] = this->forward.y;
  viewMatrix.m[2][2] = this->forward.z;
  viewMatrix.m[3][2] = -VecMath::DotProduct(this->position, this->forward);
  viewMatrix.m[3][3] = 1;

  return viewMatrix;
}