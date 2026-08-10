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

void Camera::MoveCameraWorld(const vec3d &move) // not so sure about this
{
  this->position += move;
}

void Camera::TeleportCameraWorld(const vec3d &location)
{
  this->position = location;
}

void Camera::MoveCameraLocal(const vec3d &location)
{
  vec3d right = this->GetRight();
  vec3d up = this->GetUp();

  this->position += (right * location.x);
  this->position += (up * location.y);
  this->position += (this->forward * location.z);
}

void Camera::TeleportCameraLocal(const vec3d &location)
{
  vec3d right = this->GetRight();
  vec3d up = this->GetUp();

  this->position = (right * location.x) + (up * location.y) + (this->forward * location.z);
}

void Camera::ChangeDiraction(const vec3d &theta)
{
  vec3d baseForward = {0, 0, 1};
  mat4x4 CameraRot = VecMath::MakeIdentity();
  mat4x4 rotX = VecMath::MakeIdentity();
  mat4x4 rotY = VecMath::MakeIdentity();
  this->pitch += theta.x;
  this->yaw += theta.y;

  VecMath::RotationX(rotX, this->pitch * 3.14159f / 180.0f);
  VecMath::RotationY(rotY, this->yaw * 3.14159f / 180.0f);
  CameraRot = rotX * rotY;

  this->forward = VecMath::MultiplyMatrixVector(baseForward, CameraRot);
  // VecMath::Normalize(this->forward); // keep an eye
}

vec3d Camera::GetRight() const
{
  vec3d up = {0, 1, 0};
  return VecMath::GetNormalized(VecMath::CrossProduct(up, this->forward));
}

vec3d Camera::GetUp() const
{
  vec3d right = GetRight();
  return VecMath::GetNormalized(VecMath::CrossProduct(this->forward, right)); // do you even need to normalize?
}

mat4x4
Camera::MakeViewMatrix() const
{
  vec3d right = this->GetRight();
  vec3d newUp = this->GetUp();

  std::cout << "forward: " << forward.x << "/" << forward.y << "/" << forward.z << "\n";

  mat4x4 viewMatrix;
  viewMatrix.m[0][0] = right.x;
  viewMatrix.m[1][0] = right.y;
  viewMatrix.m[2][0] = right.z;
  viewMatrix.m[3][0] = -VecMath::DotProduct(right, this->position);
  viewMatrix.m[0][1] = newUp.x;
  viewMatrix.m[1][1] = newUp.y;
  viewMatrix.m[2][1] = newUp.z;
  viewMatrix.m[3][1] = -VecMath::DotProduct(newUp, this->position);
  viewMatrix.m[0][2] = this->forward.x;
  viewMatrix.m[1][2] = this->forward.y;
  viewMatrix.m[2][2] = this->forward.z;
  viewMatrix.m[3][2] = -VecMath::DotProduct(this->forward, this->position);
  viewMatrix.m[3][3] = 1;

  return viewMatrix;
}