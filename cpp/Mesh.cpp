#include "Mesh.h"
#include <sstream>
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
        int a, b, c;

        if (s >> a >> b >> c)
        {
          tris.push_back({verts[a - 1], verts[b - 1], verts[c - 1]});
        }
      }
    }
  }
  else
  {
    std::cerr << "Error code: " << strerror(errno);
  }
}