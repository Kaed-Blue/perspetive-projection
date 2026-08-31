#pragma once
#include <sdl3/sdl.h> // maybe using sdl would be better?
#include <iostream>

class ScopeTimer
{
private:
  Uint64 start;
  const char *name;

public:
  ScopeTimer(const char *name) : name(name)
  {
    this->start = SDL_GetPerformanceCounter();
  }

  ~ScopeTimer()
  {
    auto end = SDL_GetPerformanceCounter();

    auto duration = static_cast<double>(end - start) / SDL_GetPerformanceFrequency();

    std::cout << this->name << ": " << duration * 1000000 << " us\n";
  }
};