#include "Renderer.h"

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

vec3d
Renderer::MultiplyMatrixVector(const vec3d& i, const mat4x4& m)
{
  vec3d o;
  o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
  o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
  o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];
  float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];

  if (w != 0.0f) {
    o.x /= w;
    o.y /= w;
    o.z /= w;
  }
  return o;
}

mat4x4
Renderer::MultiplyMatrices(const mat4x4& m1, const mat4x4& m2)
{
  mat4x4 res;

  for (int row = 0; row < 4; row++) {
    for (int col = 0; col < 4; col++) {

      res.m[row][col] =
        m1.m[row][0] * m2.m[0][col] + m1.m[row][1] * m2.m[1][col] +
        m1.m[row][2] * m2.m[2][col] + m1.m[row][3] * m2.m[3][col];
    }
  }
  return res;
}

vec3d
Renderer::GetNormal(triangle tri) // gives normalized cross product
{
  vec3d line1, line2, normal;

  line1.x = tri.p[1].x - tri.p[0].x;
  line1.y = tri.p[1].y - tri.p[0].y;
  line1.z = tri.p[1].z - tri.p[0].z;

  line2.x = tri.p[2].x - tri.p[0].x;
  line2.y = tri.p[2].y - tri.p[0].y;
  line2.z = tri.p[2].z - tri.p[0].z;

  normal.x = (line1.y * line2.z) - (line2.y * line1.z);
  normal.y = (line1.z * line2.x) - (line2.z * line1.x);
  normal.z = (line1.x * line2.y) - (line2.x * line1.y);

  float magnitude =
    sqrtf(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
  normal.x /= magnitude;
  normal.y /= magnitude;
  normal.z /= magnitude;

  return normal;
}

void
Renderer::RotationZ(float angel)
{
  matRotZ.m[0][0] = cosf(angel);
  matRotZ.m[0][1] = sinf(angel);
  matRotZ.m[1][0] = -sinf(angel);
  matRotZ.m[1][1] = cosf(angel);
  matRotZ.m[2][2] = 1;
  matRotZ.m[3][3] = 1;
}

void
Renderer::RotationX(float angel)
{
  matRotX.m[0][0] = 1;
  matRotX.m[1][1] = cosf(angel / 2);
  matRotX.m[1][2] = sinf(angel / 2);
  matRotX.m[2][1] = -sinf(angel / 2);
  matRotX.m[2][2] = cosf(angel / 2);
  matRotX.m[3][3] = 1;
}

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
Renderer::DrawMesh(std::vector<triangle> mesh, int frame)
{
  float angel = frame * M_PI / 180;
  RotationZ(angel);
  RotationX(angel);
  mat4x4 matRotated = MultiplyMatrices(matRotZ, matRotX);

  for (auto tri : mesh) {
    triangle triProjected, triTranslated;
    for (int i = 0; i < 3; i++) {
      vec3d pointRotated = MultiplyMatrixVector(tri.p[i], matRotated);
      triTranslated.p[i] = pointRotated;
      triTranslated.p[i].z += 15.0f;
    }

    vec3d normal = GetNormal(triTranslated);
    // cout << normal.x << "/" << normal.y << "/" << normal.z << "\n";
    if (normal.z < 0) {
      for (int i = 0; i < 3; i++) {
        triProjected.p[i] = MultiplyMatrixVector(triTranslated.p[i], matproj);
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

      DrawTriangle(sdlrenderer,
                   triProjected.p[0].x,
                   triProjected.p[0].y,
                   triProjected.p[1].x,
                   triProjected.p[1].y,
                   triProjected.p[2].x,
                   triProjected.p[2].y);
    }
  }
}