#pragma once
#include "types.h"
#include "Clipper.h"
#include <SDL3/SDL.h>
#include <math.h>
#include <vector>

class Mesh;
class Camera;
class Ilumination;
struct triInfo;

class Renderer
{
private:
  float Near = 0.1f;
  float Far = 500.0f;
  float Fov = 90.0f;
  float FovRad = 1.0f / tanf(Fov * 0.5f / 180.0f * 3.14159f);
  float aspectRatio;
  int w, h;
  Clipper clipper;
  SDL_Renderer *sdlrenderer;
  mat4x4 matProj;

public:
  Renderer(SDL_Renderer* sdlrenderer);

  float GetAspectRatio(SDL_Renderer *sdlrenderer);

  void DrawLine3D(const vec3d &p1, const vec3d &p2, const Camera &camera);

  void DrawGrid3D(const Camera &Camera);

  mat4x4 GetRotaionMatrix(const vec3d &angels);

  void NdcToPixels(vec3d &v);
  void NdcToPixels(triangle &tri);

  mat4x4 MakeWorldMat(const vec3d &objPos, const vec3d &angels);

  triangle TransformTriangle(const triangle &tri, const mat4x4 &mat);

  void DrawTriangle(SDL_Renderer* sdlrenderer,
                    int a_x,
                    int a_y,
                    int b_x,
                    int b_y,
                    int c_x,
                    int c_y);

  triInfo GetTriangleInfo(const triangle &tri, const Camera &camera);

  void DrawMesh(const Mesh &mesh, const vec3d &angels, const Camera &camera, const vec3d &objPos, const std::vector<Ilumination> &iluminations);

  void Rasterize(const std::vector<triangle> &triangles);
};