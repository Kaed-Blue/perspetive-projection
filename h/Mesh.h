#pragma once
#include "types.h"
#include <vector>


class Mesh
{
public:
  std::vector<triangle> tris;

  void CreateCube();
};