#pragma once
#include "types.h"
#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>

class Mesh
{
public:
  std::vector<triangle> tris;

  void CreateCube();

  void LoadFromObj(std::string fileName);
};