#include "Debug.h"
#include "Renderer.h"

void Debug::DrawGrid3D(Renderer &renderer, const Camera &Camera)
{
  const int size = 1;
  const float spacing = 10.0f;

  // Lines parallel to X
  for (int y = -size; y <= size; y++)
  {
    for (int z = -size; z <= size; z++)
    {
      renderer.DrawLine3D(
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
      renderer.DrawLine3D(
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
      renderer.DrawLine3D(
          {x * spacing, y * spacing, -size * spacing},
          {x * spacing, y * spacing, size * spacing},
          Camera);
    }
  }
}