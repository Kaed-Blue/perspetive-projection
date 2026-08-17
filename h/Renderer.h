#pragma once
#include "types.h"
#include <SDL3/SDL.h>
#include <math.h>
#include <vector>

class Mesh;
class Camera;

class Renderer
{
private:
  float Near = 0.1f;
  float Far = 1000.0f;
  float Fov = 90.0f;
  float FovRad = 1.0f / tanf(Fov * 0.5f / 180.0f * 3.14159f);
  int w, h;
  // float aspectRatio;
  SDL_Renderer *sdlrenderer;
  mat4x4 matProj;
  mat4x4 matRotX, matRotY, matRotZ;

public:
  Renderer(SDL_Renderer* sdlrenderer);

  void DrawLine3D(const vec3d &p1, const vec3d &p2, const Camera &camera);

  void DrawGrid3D(const Camera &Camera);

  
  mat4x4 GetRotaionMatrix(const vec3d &angels);
  
  void NdcToPixels(vec3d &v);
  int ClipAgainstPlain(const vec3d &pointOnPlain, const vec3d &normal, const triangle &tri, triangle &outTri1, triangle &outTri2);

  void DrawTriangle(SDL_Renderer* sdlrenderer,
                    int a_x,
                    int a_y,
                    int b_x,
                    int b_y,
                    int c_x,
                    int c_y);

  void DrawMesh(Mesh mesh, vec3d angels, const Camera &camera, vec3d objPos);
};