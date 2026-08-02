#pragma once
#include "types.h"
#include <cstring>
#include <vector>
#include <sstream>
#include <iostream>

struct FaceValue
{
  int vertexNum;
  int texcoord;
  int normal;
};

class Mesh
{
public:
  std::vector<triangle> tris;

  void CreateCube();

  void LoadFromObj(std::string fileName);

  FaceValue ParseFace(std::string word, char delimiter);
};
