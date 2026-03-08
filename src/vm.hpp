#pragma once

#include <array>
#include <cstdint>

#define MEMORY_SIZE 2048

class VM {
private:
  std::array<uint8_t, MEMORY_SIZE> memory;

public:
  VM();

  void reset();

  const size_t memory_size();
};
