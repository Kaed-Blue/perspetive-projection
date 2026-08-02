#include "Mesh.h"
#include <fstream>

void
Mesh::CreateCube()
{
  tris = {

    // SOUTH face
    { { { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 1.0f, 0.0f } } },
    { { { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } } },

    // EAST face
    { { { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f } } },
    { { { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f, 1.0f } } },

    // NORTH face
    { { { 1.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f, 1.0f } } },
    { { { 1.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f } } },

    // WEST face
    { { { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f } } },
    { { { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } } },

    // TOP face
    { { { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f } } },
    { { { 0.0f, 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 0.0f } } },

    // BOTTOM face
    { { { 1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f } } },
    { { { 1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } } }
  };
};

void Mesh::LoadFromObj(std::string fileName)
{
  std::ifstream obj(fileName);
  if (obj)
  {
    std::vector<vec3d> verts;
    std::string line;

    while (std::getline(obj, line))
    {
      if (line.empty())
      {
        continue;
      }

      std::stringstream s(line);
      std::string type;

      s >> type;

      if (type == "v")
      {
        float x, y, z;
        if (s >> x >> y >> z)
        {
          verts.push_back({x, y, z});
        }
      }

      else if (type == "f")
      {
        std::string a, b, c;

        if (s >> a >> b >> c)
        {
          tris.push_back({verts[ParseFace(a, '/').vertexNum - 1], verts[ParseFace(b, '/').vertexNum - 1], verts[ParseFace(c, '/').vertexNum - 1]});
        }
      }
    }
  }
  else
  {
    std::cerr << "Error code: " << strerror(errno);
  }
}

FaceValue Mesh::ParseFace(std::string word, char delimiter)
{
  FaceValue faceValue;
  std::string token;
  std::stringstream ss(word);

  std::getline(ss, token, delimiter);
  faceValue.vertexNum = std::stoi(token);

  std::getline(ss, token, delimiter);
  faceValue.texcoord = std::stoi(token);

  std::getline(ss, token, delimiter);
  faceValue.normal = std::stoi(token);

  return faceValue;
}
