#include "vm.hpp"
#include <cassert>
#include <cstdint>
#include <format>
#include <iterator>
#include <stdexcept>

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

  switch (opcode) {
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
