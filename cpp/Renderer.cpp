#include "Renderer.h"
#include "VecMath.h"

Renderer::Renderer(SDL_Renderer* sdlrenderer)
  : sdlrenderer(sdlrenderer)
{
  int w, h;
  SDL_GetRenderOutputSize(sdlrenderer, &w, &h);
  aspectRatio = static_cast<float>(w) / h;
  fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);
  matproj.m[0][0] = aspectRatio * fFovRad;
  matproj.m[1][1] = fFovRad;
  matproj.m[2][2] = fFar / (fFar - fNear);
  matproj.m[2][3] = (-fFar * fNear) / (fFar - fNear);
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
  SDL_RenderLine(sdlrenderer, a_x, a_y, b_x, b_y);
  SDL_RenderLine(sdlrenderer, b_x, b_y, c_x, c_y);
  SDL_RenderLine(sdlrenderer, c_x, c_y, a_x, a_y);
}

mat4x4 operator*(const mat4x4 &mat1, const mat4x4 &mat2) // operator overloading
{
  return VecMath::MultiplyMatrices(mat1, mat2);
}

void
Renderer::DrawMesh(std::vector<triangle> mesh, int frame, vec3d cameraPos)
{
  float angel = frame * M_PI / 180;
  VecMath::RotationX(matRotX, angel, 0.7);
  VecMath::RotationY(matRotY, angel, 0.5);
  VecMath::RotationZ(matRotZ, angel, 1);

  mat4x4 matRotated = matRotX * matRotY * matRotZ; // using "*" as MultiplyMatrices

  for (auto tri : mesh) {
    triangle triProjected, triTranslated;
    for (int i = 0; i < 3; i++) {
      vec3d pointRotated = VecMath::MultiplyMatrixVector(tri.p[i], matRotated);
      triTranslated.p[i] = pointRotated;
      triTranslated.p[i].z += 15.0f;
    }

    vec3d normal = VecMath::GetNormal(triTranslated);
    vec3d cameraRay = VecMath::SubtractVector(triTranslated.p[0], cameraPos);
    float dotProduct = VecMath::DotProduct(normal, cameraRay);

    if (dotProduct > 0) {
      for (int i = 0; i < 3; i++) {
        triProjected.p[i] =
            VecMath::MultiplyMatrixVector(triTranslated.p[i], matproj);
      }

      int w, h;
      SDL_GetRenderOutputSize(sdlrenderer, &w, &h);
      for (int i = 0; i < 3; i++) {
        triProjected.p[i].x += 1.0f;
        triProjected.p[i].y += 1.0f;
        triProjected.p[i].x *= 0.5f * (float)w; // centers to x
        triProjected.p[i].y *= 0.5f * (float)h; // centers to y
        // triProjected.p[i].x += 100.0f;            // moves everything in x
        // triProjected.p[i].y += 100.0f;            // moves everything in y
      }

      SDL_FColor Lum = Rilumination.ShadowValue(normal);

      SDL_Vertex vertex[3];
      vertex[0].position = { triProjected.p[0].x, triProjected.p[0].y };
      vertex[0].color = Lum;
      vertex[1].position = { triProjected.p[1].x, triProjected.p[1].y };
      vertex[1].color = Lum;
      vertex[2].position = { triProjected.p[2].x, triProjected.p[2].y };
      vertex[2].color = Lum;

      SDL_RenderGeometry(sdlrenderer, NULL, vertex, 3, NULL, 0);

      // DrawTriangle(sdlrenderer,
      //              triProjected.p[0].x,
      //              triProjected.p[0].y,
      //              triProjected.p[1].x,
      //              triProjected.p[1].y,
      //              triProjected.p[2].x,
      //              triProjected.p[2].y);
    }
  }
}