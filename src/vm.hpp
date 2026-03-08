#pragma once

#include <array>
#include <cstdint>
#include <vector>

#define MEMORY_SIZE 2048
#define MAX_PROGRAM_SIZE 128

class VM {
public:
  std::array<uint8_t, MEMORY_SIZE> memory;

  VM();

  void reset();

  uint8_t operator[](int idx);

  const size_t memory_size();

  void load_program(std::vector<uint8_t> program);
};
