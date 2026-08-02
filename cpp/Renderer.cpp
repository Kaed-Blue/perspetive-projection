#include "Renderer.h"
#include "VecMath.h"
#include "algorithm"
#include "Mesh.h"

Renderer::Renderer(SDL_Renderer* sdlrenderer)
  : sdlrenderer(sdlrenderer)
{
  int w, h;
  SDL_GetRenderOutputSize(sdlrenderer, &w, &h);
  aspectRatio = static_cast<float>(w) / static_cast<float>(h);
  FovRad = 1.0f / tanf(Fov * 0.5f / 180.0f * 3.14159f);
  matproj.m[0][0] = 1 / aspectRatio * FovRad;
  matproj.m[1][1] = FovRad;
  matproj.m[2][2] = Far / (Far - Near);
  matproj.m[2][3] = (-Far * Near) / (Far - Near);
  matproj.m[3][2] = 1.0f;
};

void
Renderer::DrawTriangle(SDL_Renderer* sdlrenderer,
                       int a_x,
                       int a_y,
                       int b_x,
                       int b_y,
                       int c_x,
                       int c_y)
{
  // SDL_SetRenderDrawColor(sdlrenderer, 0, 0, 0, 1);
  SDL_RenderLine(sdlrenderer, a_x, a_y, b_x, b_y);
  SDL_RenderLine(sdlrenderer, b_x, b_y, c_x, c_y);
  SDL_RenderLine(sdlrenderer, c_x, c_y, a_x, a_y);
  // SDL_SetRenderDrawColor(sdlrenderer, 1, 1, 1, 1);
}

mat4x4 operator*(const mat4x4 &mat1, const mat4x4 &mat2) // operator overloading
{
  return VecMath::MultiplyMatrices(mat1, mat2);
}

void Renderer::DrawMesh(Mesh mesh, int frame, vec3d cameraPos, vec3d objPos)
{
  std::vector<triangle> vectriprojeted;
  mat4x4 worldMat = VecMath::MakeIdentity();
  float angel = frame * M_PI / 180;

  // Rotation
  VecMath::RotationX(matRotX, angel, 0.7);
  VecMath::RotationY(matRotY, angel, 0.5);
  VecMath::RotationZ(matRotZ, angel, 1);

  // Translation
  mat4x4 matTrans = VecMath::TranslateMatrix(objPos.x, objPos.y, objPos.z);

  worldMat = matRotX * matRotY * matRotZ * matTrans; // using "*" as MultiplyMatrices

  for (triangle tri : mesh.tris)
  {
    triangle triProjected, triTransformed;
    for (int i = 0; i < 3; i++) {
      triTransformed.p[i] = VecMath::MultiplyMatrixVector(tri.p[i], worldMat);
    }

    vec3d normal = VecMath::GetVectorNormal(triTransformed);
    vec3d cameraRay = VecMath::SubtractVector(triTransformed.p[0], cameraPos);
    float dotProduct = VecMath::DotProduct(normal, cameraRay);

    if (dotProduct > 0) {
      for (int i = 0; i < 3; i++) {
        triProjected.p[i] = VecMath::MultiplyMatrixVector(triTransformed.p[i], matproj);
      }

      int w, h;
      SDL_GetRenderOutputSize(sdlrenderer, &w, &h); // TODO: put in a func this is messy
      for (int i = 0; i < 3; i++) {
        triProjected.p[i].x += 1.0f; // moves from [-1. 1] space to [0, 2]
        triProjected.p[i].y += 1.0f;
        triProjected.p[i].x *= 0.5f * (float)w; // scales into pixel [0, w] and [0, h]
        triProjected.p[i].y *= 0.5f * (float)h;
        // triProjected.p[i].x += 100.0f;            // moves everything in x
        // triProjected.p[i].y += 100.0f;            // moves everything in y
      }

      SDL_FColor Lum = Rilumination.ShadowValue(normal);
      triProjected.p->color = Lum;

      vectriprojeted.push_back(triProjected);
    }
  }

  sort(vectriprojeted.begin(), vectriprojeted.end(),
       [](const triangle &tri1, const triangle &tri2) // lambda
       {float z1Avg = tri1.p[0].z + tri1.p[1].z + tri1.p[2].z;
        float z2Avg = tri2.p[0].z + tri2.p[1].z + tri2.p[2].z;
        return z1Avg < z2Avg; });

  for (triangle triProjected : vectriprojeted)
  {
    SDL_Vertex vertex[3];
    vertex[0].position = {triProjected.p[0].x, triProjected.p[0].y};
    vertex[0].color = triProjected.p->color;
    vertex[1].position = {triProjected.p[1].x, triProjected.p[1].y};
    vertex[1].color = triProjected.p->color;
    vertex[2].position = {triProjected.p[2].x, triProjected.p[2].y};
    vertex[2].color = triProjected.p->color;

    SDL_RenderGeometry(sdlrenderer, NULL, vertex, 3, NULL, 0);

    //   DrawTriangle(sdlrenderer,
    //                triProjected.p[0].x,
    //                triProjected.p[0].y,
    //                triProjected.p[1].x,
    //                triProjected.p[1].y,
    //                triProjected.p[2].x,
    //                triProjected.p[2].y);
    //
  }
}