#pragma once

class Camera;
class Renderer;

class Debug
{
public:
  static void DrawGrid3D(Renderer &renderer, const Camera &Camera);
};