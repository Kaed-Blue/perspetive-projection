#pragma once
#include <chrono>
#include <iostream>

class ScopeTimer
{
private:
  std::chrono::high_resolution_clock::time_point start;
  const char *name;

public:
  ScopeTimer(const char *name) : name(name)
  {
    this->start = std::chrono::high_resolution_clock::now();
  }

  ~ScopeTimer()
  {
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << this->name << ": " << duration.count() << " us\n";
  }
};