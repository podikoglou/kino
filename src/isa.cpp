#include "isa.hpp"

const uint8_t extract_opcode(Instruction instruction) {
  return (instruction & 0xFF000000) >> 24;
}
