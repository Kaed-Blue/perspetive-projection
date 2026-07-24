#include "Engine.h"
#include <iostream>

using namespace std;

int main()
{
  cout << "1";
  Engine engine;
  cout << "2";
  engine.Initialize();
  engine.run();

  cout << "main finished";
  return 0;
}