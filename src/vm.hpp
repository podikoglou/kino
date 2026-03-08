#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <vector>

#define MEMORY_SIZE 2048
#define MAX_PROGRAM_SIZE 128

class VM {
public:
  std::array<uint8_t, MEMORY_SIZE> memory;

  // an index to `memory`, pointing to the current instruction
  size_t pc;

  // the instruction currently being executed is stored here
  // invariant: this should be memory[pc]
  uint8_t ir;

  VM();

  void reset();

  void fetch();
  void execute();

  uint8_t operator[](int idx);

  const size_t memory_size();

  void load_program(std::vector<uint8_t> program);
};
