#include "vm.hpp"
#include <format>
#include <iterator>
#include <stdexcept>

VM::VM() { this->reset(); }

void VM::fetch() { this->ir = this->memory[this->pc]; }

void VM::execute() {}

void VM::reset() { this->memory.fill(0x00); }

uint8_t VM::operator[](int idx) { return this->memory[idx]; }

const size_t VM::memory_size() { return MEMORY_SIZE; }

void VM::load_program(std::vector<uint8_t> program) {
  // ensure program is not too long
  if (program.size() > MAX_PROGRAM_SIZE)
    throw std::length_error(std::format(
        "program too long (must be up to {} bytes)", MAX_PROGRAM_SIZE));

  for (int i = 0; i < std::size(program); i++) {
    this->memory[i] = program[i];
  }
}
