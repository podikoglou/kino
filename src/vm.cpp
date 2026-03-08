#include "vm.hpp"
#include <cassert>
#include <cstdint>
#include <format>
#include <iterator>
#include <stdexcept>
#include <variant>

// Documented in ISA.MD
constexpr uint8_t OP_PUSHC = 0x00;
constexpr uint8_t OP_PUSHV = 0x01;
constexpr uint8_t OP_STORE = 0x02;
constexpr uint8_t OP_ADD = 0x06;
constexpr uint8_t OP_SUB = 0x07;
constexpr uint8_t OP_MULT = 0x08;
constexpr uint8_t OP_DIV = 0x09;

VM::VM() { this->reset(); }

void VM::fetch() {
  // this invariant should always be true, since we don't load programs after a
  // certain address (128 as of writing)
  assert(this->memory_size() > this->pc + 3);

  uint8_t a = static_cast<uint32_t>(this->memory[this->pc]);
  uint8_t b = static_cast<uint32_t>(this->memory[this->pc + 1]);
  uint8_t c = static_cast<uint32_t>(this->memory[this->pc + 2]);
  uint8_t d = static_cast<uint32_t>(this->memory[this->pc + 3]);

  this->ir = d | (c << 8) | (b << 16) | (a << 24);
}

void VM::execute() {
  const uint32_t instruction = this->ir;
  const uint8_t opcode = (instruction & 0xFF000000) >> 24;

  value value_a, value_b;

  switch (opcode) {
  case OP_ADD:
  case OP_SUB:
  case OP_MULT:
  case OP_DIV:
    if (this->stack.size() < 2) {
      throw std::runtime_error(
          "not enough values to operate on (must have at least 2)");
    }

    // pop two values from the stack
    value_a = this->stack.top();
    this->stack.pop();

    value_b = this->stack.top();
    this->stack.pop();

    try {
      uint32_t a = std::get<uint32_t>(value_a);
      uint32_t b = std::get<uint32_t>(value_b);

      uint32_t result;

      // TODO: handle overflows and floating point numbers

      // perform actual calculation
      switch (opcode) {
      case OP_ADD:
        result = a + b;
        break;
      case OP_SUB:
        result = a - b;
        break;
      case OP_MULT:
        result = a * b;
        break;
      case OP_DIV:
        result = a / b;
        break;
      }

      // push back to stack
    } catch (const std::bad_variant_access &ex) {
      throw std::runtime_error(std::format("type mismatch"));
    }

    break;

  default:
    throw std::runtime_error(
        std::format("invalid opcode: {:#x} (full instruction: {:#x})", opcode,
                    instruction));
  }
}

void VM::reset() { this->memory.fill(0x00); }

void VM::pc_inc() { this->pc += 4; }

uint8_t VM::operator[](int idx) const { return this->memory[idx]; }

size_t VM::memory_size() const { return MEMORY_SIZE; }

void VM::load_program(std::vector<uint8_t> program) {
  // ensure program is not too long
  if (program.size() > MAX_PROGRAM_SIZE)
    throw std::length_error(std::format(
        "program too long (must be up to {} bytes)", MAX_PROGRAM_SIZE));

  for (int i = 0; i < std::size(program); i++) {
    this->memory[i] = program[i];
  }
}
