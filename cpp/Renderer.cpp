#include "Renderer.h"
#include <iostream>

Renderer::Renderer(SDL_Renderer* sdlrenderer)
  : sdlrenderer(sdlrenderer)
{
  int w, h;
  SDL_GetRenderOutputSize(sdlrenderer, &w, &h);
  float aspectRatio = w / h;
  float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);
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

void
Renderer::DrawMesh(std::vector<triangle> mesh, int frame, vec3d cameraPos)
{
  float angel = frame * M_PI / 180;
  RvecMath.RotationZ(matRotZ, angel);
  RvecMath.RotationX(matRotX, angel);

  mat4x4 matRotated = RvecMath.MultiplyMatrices(matRotZ, matRotX);

  for (auto tri : mesh) {
    triangle triProjected, triTranslated;
    for (int i = 0; i < 3; i++) {
      vec3d pointRotated = RvecMath.MultiplyMatrixVector(tri.p[i], matRotated);
      triTranslated.p[i] = pointRotated;
      triTranslated.p[i].z += 15.0f;
    }

    vec3d normal = RvecMath.GetNormal(triTranslated);
    vec3d cameraRay = RvecMath.SubtractVector(triTranslated.p[0], cameraPos);
    float dotProduct = RvecMath.DotProduct(normal, cameraRay);

    if (dotProduct > 0) {
      for (int i = 0; i < 3; i++) {
        triProjected.p[i] =
          RvecMath.MultiplyMatrixVector(triTranslated.p[i], matproj);
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

      vec3d lightRay = Rilumination.GetLightRay();
      RvecMath.Normalize(lightRay);
      float LumDP = RvecMath.DotProduct(normal, lightRay);
      SDL_FColor Lum = Rilumination.ShadowValue(
        LumDP); // TODO: make ShadowValue do the 3 lines above

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