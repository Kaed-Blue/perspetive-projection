#include "Renderer.h"
#include "VecMath.h"
#include "algorithm"
#include "Mesh.h"
#include "Camera.h"
#include "Ilumination.h"
#include <array>

struct triInfo
{
  vec3d normal;
  bool backface;
};

Renderer::Renderer(SDL_Renderer *sdlrenderer)
    : sdlrenderer(sdlrenderer),
      aspectRatio(GetAspectRatio(sdlrenderer)),
      clipper(Fov, aspectRatio, Near, Far)
{
  matProj.m[0][0] = 1 / aspectRatio * FovRad;
  matProj.m[1][1] = FovRad;
  matProj.m[2][2] = Far / (Far - Near);
  matProj.m[3][2] = (-Far * Near) / (Far - Near);
  matProj.m[2][3] = 1.0f;
  matProj.m[3][3] = 0.0f;
};

float Renderer::GetAspectRatio(SDL_Renderer *sdlrenderer)
{
  SDL_GetRenderOutputSize(sdlrenderer, &this->w, &this->h);
  return static_cast<float>(w) / static_cast<float>(h);
}

void Renderer::DrawLine3D(const vec3d &p1, const vec3d &p2, const Camera &camera) // not sure about refrencing the camera
{
  mat4x4 viewmatrix = camera.GetViewMatrix();
  vec3d p1viewed = VecMath::MultiplyMatrixVector(p1, viewmatrix);
  vec3d p2viewed = VecMath::MultiplyMatrixVector(p2, viewmatrix);

  vec3d p1Clipped, p2Clipped;

  if (!Clipper::ClipAgainstPlain({0.0f, 0.0f, this->Near}, {0.0f, 0.0f, 1.0f}, p1viewed, p2viewed, p1Clipped, p2Clipped))
  {
    return;
  }

  vec3d p1Projected = VecMath::MultiplyMatrixVector(p1Clipped, matProj);
  vec3d p2Projected = VecMath::MultiplyMatrixVector(p2Clipped, matProj);

  NdcToPixels(p1Projected);
  NdcToPixels(p2Projected);

  SDL_RenderLine(this->sdlrenderer, p1Projected.x, h - p1Projected.y, p2Projected.x, h - p2Projected.y); // SDL is y down! beware!
}

void Renderer::DrawGrid3D(const Camera &Camera) // TODO: This shouldn't be here
{
  const int size = 1;
  const float spacing = 10.0f;

  // Lines parallel to X
  for (int y = -size; y <= size; y++)
  {
    for (int z = -size; z <= size; z++)
    {
      this->DrawLine3D(
          {-size * spacing, y * spacing, z * spacing},
          {size * spacing, y * spacing, z * spacing},
          Camera);
    }
  }

  // Lines parallel to Y
  for (int x = -size; x <= size; x++)
  {
    for (int z = -size; z <= size; z++)
    {
      this->DrawLine3D(
          {x * spacing, -size * spacing, z * spacing},
          {x * spacing, size * spacing, z * spacing},
          Camera);
    }
  }

  // Lines parallel to Z
  for (int x = -size; x <= size; x++)
  {
    for (int y = -size; y <= size; y++)
    {
      this->DrawLine3D(
          {x * spacing, y * spacing, -size * spacing},
          {x * spacing, y * spacing, size * spacing},
          Camera);
    }
  }
}

void Renderer::DrawTriangle(SDL_Renderer *sdlrenderer,
                            int a_x,
                            int a_y,
                            int b_x,
                            int b_y,
                            int c_x,
                            int c_y)
{
  // SDL_SetRenderDrawColor(sdlrenderer, 0, 0, 0, 255);
  SDL_RenderLine(sdlrenderer, a_x, a_y, b_x, b_y);
  SDL_RenderLine(sdlrenderer, b_x, b_y, c_x, c_y);
  SDL_RenderLine(sdlrenderer, c_x, c_y, a_x, a_y);
  SDL_SetRenderDrawColor(sdlrenderer, 255, 255, 255, 255);
}

void Renderer::NdcToPixels(vec3d &v)
{
  v.x += 1.0f; // moves from [0, 1] space to [1, 2]
  v.y += 1.0f;
  v.x *= 0.5f * w; // scales into pixel [1, w] and [1, h]
  v.y *= 0.5f * h;
}

void Renderer::NdcToPixels(triangle &tri)
{
  for (int i = 0; i < 3; i++)
  {
    tri.p[i].x += 1.0f;
    tri.p[i].y += 1.0f;
    tri.p[i].x *= 0.5f * w;
    tri.p[i].y *= 0.5f * h;
  }
}

mat4x4 Renderer::GetRotaionMatrix(const vec3d &angels)
{
  mat4x4 rotX, rotY, rotZ;
  VecMath::RotationX(rotX, angels.x);
  VecMath::RotationY(rotY, angels.y);
  VecMath::RotationZ(rotZ, angels.z);
  return rotX * rotY * rotZ;
}

mat4x4 Renderer::MakeWorldMat(const vec3d &objPos, const vec3d &angels)
{
  mat4x4 Translation = VecMath::TranslateMatrix(objPos.x, objPos.y, objPos.z);
  mat4x4 rots = this->GetRotaionMatrix(angels);
  return Translation * rots;
}

triangle Renderer::TransformTriangle(const triangle &tri, const mat4x4 &mat)
{
  triangle transformed;
  for (int i = 0; i < 3; i++)
  {
    transformed.p[i] = VecMath::MultiplyMatrixVector(tri.p[i], mat);
  }
  return transformed;
}

triInfo Renderer::GetTriangleInfo(const triangle &tri, const Camera &camera)
{
  vec3d line1, line2;
  line1 = tri.p[1] - tri.p[0];
  line2 = tri.p[2] - tri.p[0];

  vec3d normal = VecMath::GetNormalized(VecMath::CrossProduct(line1, line2));
  vec3d cameraRay = tri.p[0] - camera.GetCameraPos();

  return {normal, VecMath::DotProduct(normal, cameraRay) > 0};
}

void Renderer::DrawMesh(const Mesh &mesh, const vec3d &angels, const Camera &camera, const vec3d &objPos, const std::vector<Ilumination> &iluminations) // FIXME: this function is doing too much
{
  std::vector<triangle> vectriprojeted;
  vectriprojeted.reserve(mesh.tris.size());

  // Combine every transformation in the same matrix
  mat4x4 worldMat = this->MakeWorldMat(objPos, angels);

  for (const triangle &tri : mesh.tris)
  {
    triangle triProjected, triTransformed, triViewed;

    // Apply translation and roatation
    triTransformed = this->TransformTriangle(tri, worldMat);

    triInfo info = GetTriangleInfo(triTransformed, camera);

    // Backface culling
    if (info.backface)
      continue;

    // Bring into camera view
    triViewed = this->TransformTriangle(triTransformed, camera.GetViewMatrix());

    // Cut to clip-space
    ClipResult triClipped = clipper.ClipSpace(triViewed);

    // Project into screen space (3D -> 2D)
    for (int i = 0; i < triClipped.count; i++)
    {
      triProjected = this->TransformTriangle(triClipped.triangles[i], this->matProj);
      NdcToPixels(triProjected);

      // Lighting
      triProjected.p->color = iluminations.at(0).ShadowValue(info.normal); // TODO: Add color merge

      // Store projected tris
      vectriprojeted.push_back(triProjected);
    }
  }

  // Sorting projected tris from back(+z) to front(-z)
  sort(vectriprojeted.begin(), vectriprojeted.end(), // TODO: Use z buffer(depth buffer)
       [](const triangle &tri1, const triangle &tri2)
       {
         float z1Avg = tri1.p[0].z + tri1.p[1].z + tri1.p[2].z;
         float z2Avg = tri2.p[0].z + tri2.p[1].z + tri2.p[2].z;
         return z1Avg > z2Avg;
       });

  this->Rasterize(vectriprojeted);
}

void Renderer::Rasterize(const std::vector<triangle> &triangles)
{
  // Assigning to SDL_Vertex so it can be drawn by RenderGeometry
  for (triangle tri : triangles)
  {

    SDL_Vertex vertex[3];
    vertex[0].position = {tri.p[0].x, h - tri.p[0].y}; // SDL is y down
    vertex[0].color = tri.p->color;
    vertex[1].position = {tri.p[1].x, h - tri.p[1].y};
    vertex[1].color = tri.p->color;
    vertex[2].position = {tri.p[2].x, h - tri.p[2].y};
    vertex[2].color = tri.p->color;

    // Drawing
    SDL_RenderGeometry(sdlrenderer, NULL, vertex, 3, NULL, 0);

    // DrawTriangle(sdlrenderer,
    //              tri.p[0].x,
    //              h - tri.p[0].y,
    //              tri.p[1].x,
    //              h - tri.p[1].y,
    //              tri.p[2].x,
    //              h - tri.p[2].y);
  }
}