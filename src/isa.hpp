#pragma once

#include <cstdint>
#include <string_view>

typedef uint32_t Instruction;

// Documented in ISA.MD
enum class Opcode : uint8_t {
  PUSHC = 0x00,
  PUSHV = 0x01,
  STORE = 0x02,
  ADD = 0x06,
  SUB = 0x07,
  MULT = 0x08,
  DIV = 0x09
};

const Opcode extract_opcode(Instruction instruction);
const std::string_view opcode_name(Opcode opcode);
