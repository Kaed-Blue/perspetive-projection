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
  mat4x4 viewmatrix = camera.GetViewMatrix();
  vec3d p1viewed = VecMath::MultiplyMatrixVector(p1, viewmatrix);
  vec3d p2viewed = VecMath::MultiplyMatrixVector(p2, viewmatrix);

  vec3d p1Projected = VecMath::MultiplyMatrixVector(p1viewed, matProj);
  vec3d p2Projected = VecMath::MultiplyMatrixVector(p2viewed, matProj);

  NdcToPixels(p1Projected);
  NdcToPixels(p2Projected);

  SDL_RenderLine(this->sdlrenderer, p1Projected.x, h - p1Projected.y, p2Projected.x, h - p2Projected.y); // SDL is y down! beware!
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

mat4x4 Renderer::GetRotaionMatrix(const vec3d &angels)
{
  mat4x4 rotX, rotY, rotZ;
  VecMath::RotationX(rotX, angels.x);
  VecMath::RotationY(rotY, angels.y);
  VecMath::RotationZ(rotZ, angels.z);
  return rotX * rotY * rotZ;
}

int Renderer::ClipAgainstPlain(const vec3d &pointOnPlain, const vec3d &normal, const triangle &inTri, triangle &outTri1, triangle &outTri2)
{
  vec3d inside[3];
  int insideCount = 0;
  vec3d outside[3];
  int outsideCount = 0;
  float distanceToPlain = VecMath::DotProduct(normal, pointOnPlain); // didn't multi it by -1 to make it dist

  for (int i = 0; i < 3; i++)
  {
    float pdistance = VecMath::DotProduct(normal, inTri.p[i]);
    if (distanceToPlain <= pdistance)
    {
      inside[insideCount++] = inTri.p[i];
    }
    else
    {
      outside[outsideCount++] = inTri.p[i];
    }
  }

  vec3d intersection[3];
  switch (insideCount)
  {
  case 0:
    return 0;

  case 1:
    VecMath::PlainIntersect(pointOnPlain, normal, inside[0], outside[0], intersection[0]);
    VecMath::PlainIntersect(pointOnPlain, normal, inside[0], outside[1], intersection[1]);
    outTri1 = {inside[0], intersection[0], intersection[1]};
    return 1;

  case 2:
    VecMath::PlainIntersect(pointOnPlain, normal, inside[0], outside[0], intersection[0]);
    VecMath::PlainIntersect(pointOnPlain, normal, inside[1], outside[0], intersection[1]);

    outTri1 = {inside[0], inside[1], intersection[0]};
    outTri2 = {inside[1], intersection[0], intersection[1]};
    return 2;

  case 3:
    outTri1 = inTri;
    return 1;

  default:
    return -1;
  }
}

void Renderer::DrawMesh(std::vector<triangle> mesh, vec3d angels, const Camera &camera, vec3d objPos) // FIXME: this function is doing too much
{
  std::vector<triangle> vectriprojeted;
  Ilumination ilumination;
  ilumination.SetLightDir({0, 0, 1});

  // Translation of the object matrix
  mat4x4 Translation = VecMath::TranslateMatrix(objPos.x, objPos.y, objPos.z);

  // Rotation of the object matrix
  mat4x4 rots = this->GetRotaionMatrix(angels);

  // Camera view matrix
  mat4x4 viewMatrix = camera.GetViewMatrix(); // TODO: make viewMatrix a member of camera

  // Combine every transformation in the same matrix
  mat4x4 worldMat = VecMath::MakeIdentity();
  worldMat = rots * Translation;

  for (triangle tri : mesh)
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

    vec3d normal = VecMath::GetNormalized(VecMath::CrossProduct(line1, line2));
    vec3d cameraRay = triTransformed.p[0] - camera.GetCameraPos();
    float alignValue = VecMath::DotProduct(normal, cameraRay);

    // Bring into camera view
    for (int i = 0; i < 3; i++)
    {
      triViewed.p[i] = VecMath::MultiplyMatrixVector(triTransformed.p[i], viewMatrix);
    }

    // clip into camera view
    triangle clipped[2];
    int numClipped = 0;
    // near plain clipping
    numClipped = ClipAgainstPlain({0.0f, 0.0f, this->Near}, {0.0f, 0.0f, 1.0f}, triViewed, clipped[0], clipped[1]);

    // Project into screen space (3D -> 2D)
    for (int n = 0; n < numClipped; n++)
    {
      if (alignValue < 0)
      {
        for (int i = 0; i < 3; i++)
        {
          triProjected.p[i] = VecMath::MultiplyMatrixVector(clipped[n].p[i], matProj);
        }

        // NDC to pixels
        for (int i = 0; i < 3; i++)
        {
          NdcToPixels(triProjected.p[i]);
        }

        // Lighting
        SDL_FColor Lum = ilumination.ShadowValue(normal);
        triProjected.p->color = Lum;

        // Store projected tris
        vectriprojeted.push_back(triProjected);
      }
    }
  }

  // Sorting projected tris from back(+z) to front(-z)
  sort(vectriprojeted.begin(), vectriprojeted.end(),
       [](const triangle &tri1, const triangle &tri2) // lambda
       {float z1Avg = tri1.p[0].z + tri1.p[1].z + tri1.p[2].z;
        float z2Avg = tri2.p[0].z + tri2.p[1].z + tri2.p[2].z;
        return z1Avg > z2Avg; });                                          // this was the problem

  // Assining to SDL_Vertex so it can be drawn by RenderGeometry (not anymore)
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
    SDL_RenderGeometry(sdlrenderer, NULL, vertex, 3, NULL, 0); // now it's working

    // DrawTriangle(sdlrenderer,
    //              triProjected.p[0].x,
    //              h - triProjected.p[0].y,
    //              triProjected.p[1].x,
    //              h - triProjected.p[1].y,
    //              triProjected.p[2].x,
    //              h - triProjected.p[2].y);
  }
}