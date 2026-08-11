#include "Renderer.h"
#include "VecMath.h"
#include "algorithm"
#include "Mesh.h"
#include "Camera.h"
#include "Ilumination.h"

Renderer::Renderer(SDL_Renderer* sdlrenderer)
  : sdlrenderer(sdlrenderer)
{
  SDL_GetRenderOutputSize(sdlrenderer, &w, &h);
  float aspectRatio = static_cast<float>(w) / static_cast<float>(h);
  FovRad = 1.0f / tanf(Fov * 0.5f / 180.0f * 3.14159f);
  matProj.m[0][0] = 1 / aspectRatio * FovRad;
  matProj.m[1][1] = FovRad;
  matProj.m[2][2] = Far / (Far - Near);
  matProj.m[3][2] = (-Far * Near) / (Far - Near);
  matProj.m[2][3] = 1.0f;
  matProj.m[3][3] = 0.0f;
};

void Renderer::DrawLine3D(const vec3d &p1, const vec3d &p2, const Camera &camera) // not sure about refrencing the camera
{
  mat4x4 viewmatrix = camera.MakeViewMatrix();
  vec3d p1viewed = VecMath::MultiplyMatrixVector(p1, viewmatrix);
  vec3d p2viewed = VecMath::MultiplyMatrixVector(p2, viewmatrix);

  vec3d p1Projected = VecMath::MultiplyMatrixVector(p1viewed, matProj);
  vec3d p2Projected = VecMath::MultiplyMatrixVector(p2viewed, matProj);

  NdcToPixels(p1Projected);
  NdcToPixels(p2Projected);

  SDL_RenderLine(this->sdlrenderer, p1Projected.x, p1Projected.y, p2Projected.x, p2Projected.y);
}

void Renderer::DrawGrid3D(const Camera &Camera)
{
  const int size = 1;
  const float spacing = 10.0f;

  // Lines parallel to X
  for (int y = -size; y <= size; y++)
  {
    for (int z = -size; z <= size; z++)
    {
      DrawLine3D(
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
      DrawLine3D(
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
      DrawLine3D(
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
  SDL_SetRenderDrawColor(sdlrenderer, 255, 255, 255, 255);
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

void Renderer::DrawMesh(Mesh mesh, vec3d angel, const Camera &camera, vec3d objPos) // FIXME: Some partitioning maybe
{
  std::vector<triangle> vectriprojeted;
  mat4x4 worldMat = VecMath::MakeIdentity();

  // Rotation matrix
  VecMath::RotationX(matRotX, angel.x);
  VecMath::RotationY(matRotY, angel.y);
  VecMath::RotationZ(matRotZ, angel.z);

  // Translation of the object matrix
  mat4x4 matTrans = VecMath::TranslateMatrix(objPos.x, objPos.y, objPos.z);

  std::cout << "POS: " << camera.GetCameraPos().x << "/" << camera.GetCameraPos().y << "/" << camera.GetCameraPos().z << "\n";

  // Camera view matrix
  mat4x4 viewMatrix = camera.MakeViewMatrix();

  // Combine every transformation in the same matrix
  worldMat = matTrans * matRotX * matRotY * matRotZ; // using "*" as MultiplyMatrices

  for (triangle tri : mesh.tris)
  {
    triangle triProjected, triTransformed, triViewed;

    // Apply translation and roatation
    for (int i = 0; i < 3; i++)
    {
      triTransformed.p[i] = VecMath::MultiplyMatrixVector(tri.p[i], worldMat);
    }

    vec3d line1, line2;
    line1 = triTransformed.p[1] - triTransformed.p[0];
    line2 = triTransformed.p[2] - triTransformed.p[0];

    vec3d normal = VecMath::CrossProduct(line1, line2);
    VecMath::Normalize(normal);
    vec3d cameraRay = triTransformed.p[0] - camera.GetCameraPos();
    float alignValue = VecMath::DotProduct(normal, cameraRay);

    // Bring into camera view
    for (int i = 0; i < 3; i++)
    {
      triViewed.p[i] = VecMath::MultiplyMatrixVector(triTransformed.p[i], viewMatrix);
    }

    // Project into screen space (3D -> 2D)
    if (alignValue < 0)
    {
      for (int i = 0; i < 3; i++) {
        triProjected.p[i] = VecMath::MultiplyMatrixVector(triViewed.p[i], matProj);
      }

      // NDC to pixels
      for (int i = 0; i < 3; i++) {
        NdcToPixels(triProjected.p[i]);
      }

      // Lighting
      Ilumination ilumination;
      ilumination.SetLightDir({0, 0, -1});
      SDL_FColor Lum = ilumination.ShadowValue(normal);
      triProjected.p->color = Lum; // chsnge to Lum

      // Store projected tris
      vectriprojeted.push_back(triProjected);
    }
  }

  // Sorting projected tris from back(+z) to front(-z)
  sort(vectriprojeted.begin(), vectriprojeted.end(),
       [](const triangle &tri1, const triangle &tri2) // lambda
       {float z1Avg = tri1.p[0].z + tri1.p[1].z + tri1.p[2].z;
        float z2Avg = tri2.p[0].z + tri2.p[1].z + tri2.p[2].z;
        return z1Avg < z2Avg; });

  // Assining to SDL_Vertex so it can be drawn by RenderGeometry
  for (triangle triProjected : vectriprojeted)
  {

    SDL_Vertex vertex[3];
    vertex[0].position = {triProjected.p[0].x, h - triProjected.p[0].y}; // SDL is y down
    vertex[0].color = triProjected.p->color;
    vertex[1].position = {triProjected.p[1].x, h - triProjected.p[1].y};
    vertex[1].color = triProjected.p->color;
    vertex[2].position = {triProjected.p[2].x, h - triProjected.p[2].y};
    vertex[2].color = triProjected.p->color;

    // Drawing
    // SDL_RenderGeometry(sdlrenderer, NULL, vertex, 3, NULL, 0); // this has problems with view matrix or something. use DrawTriangle() for now //TODO: find or make an alternative

    DrawTriangle(sdlrenderer,
                 triProjected.p[0].x,
                 h - triProjected.p[0].y,
                 triProjected.p[1].x,
                 h - triProjected.p[1].y,
                 triProjected.p[2].x,
                 h - triProjected.p[2].y);
  }
}