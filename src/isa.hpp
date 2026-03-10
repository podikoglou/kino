#pragma once

#include <cstdint>

typedef uint32_t Instruction;

// Documented in ISA.MD
constexpr uint8_t OP_PUSHC = 0x00;
constexpr uint8_t OP_PUSHV = 0x01;
constexpr uint8_t OP_STORE = 0x02;
constexpr uint8_t OP_ADD = 0x06;
constexpr uint8_t OP_SUB = 0x07;
constexpr uint8_t OP_MULT = 0x08;
constexpr uint8_t OP_DIV = 0x09;

const uint8_t extract_opcode(Instruction instruction);
