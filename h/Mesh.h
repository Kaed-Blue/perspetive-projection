#pragma once
#include "types.h"
#include <cstring>
#include <vector>
#include <iostream>

class Mesh
{
public:
  std::vector<triangle> tris;

  void CreateCube();

  void LoadFromObj(std::string fileName);
};