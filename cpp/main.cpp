#include "Engine.h"
#include <iostream>

using namespace std;

int main()
{
  Engine engine;
  engine.Initialize();
  engine.run();
  engine.clean();

  cout << "main finished";
  return 0;
}