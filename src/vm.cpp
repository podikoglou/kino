#include "vm.hpp"
#include "isa.hpp"
#include <cassert>
#include <cstdint>
#include <exception>
#include <format>
#include <iterator>
#include <stack>
#include <stdexcept>
#include <variant>

VM::VM() { this->reset(); }

void VM::fetch() {
  // this invariant should always be true, since we don't load programs after a
  // certain address (128 as of writing)
  assert(this->memory.size() > this->pc + 3);

  uint8_t a = static_cast<uint32_t>(this->memory[this->pc]);
  uint8_t b = static_cast<uint32_t>(this->memory[this->pc + 1]);
  uint8_t c = static_cast<uint32_t>(this->memory[this->pc + 2]);
  uint8_t d = static_cast<uint32_t>(this->memory[this->pc + 3]);

  this->ir = d | (c << 8) | (b << 16) | (a << 24);

  if (this->ir == 0x00) {
    throw std::runtime_error("couldn't fetch next instruction (all zeroes)");
  }
}

void VM::execute() {
  const Instruction instruction = this->ir;
  const Opcode opcode = extract_opcode(instruction);

  switch (opcode) {
  case Opcode::PUSHC: {
    // TODO: types
    uint32_t val = instruction & 0x00FFFFFF;

    this->stack.push(val);

    break;
  }

  case Opcode::PUSHV: {
    uint32_t name = instruction & 0x00FFFFFF;

    try {
      value val = this->kv[name];

      this->stack.push(val);
    } catch (const std::exception &exception) {
      throw std::runtime_error("unknown variable");
    }

    break;
  }

  case Opcode::STORE: {
    if (this->stack.size() < 1) {
      throw std::runtime_error("no value to operate on");
    }

    uint32_t name = instruction & 0x00FFFFFF;

    value val = this->stack.top();
    this->stack.pop(); // NOTE: should we?

    this->kv[name] = val;

    break;
  }

  case Opcode::ADD:
  case Opcode::SUB:
  case Opcode::MULT:
  case Opcode::DIV: {
    if (this->stack.size() < 2) {
      throw std::runtime_error(
          "not enough values to operate on (must have at least 2)");
    }

    // pop two values from the stack
    value value_a = this->stack.top();
    this->stack.pop();

    value value_b = this->stack.top();
    this->stack.pop();

    try {
      const uint32_t a = std::get<uint32_t>(value_a);
      const uint32_t b = std::get<uint32_t>(value_b);

      uint32_t result;

      // TODO: handle overflows and floating point numbers

      // perform actual calculation
      switch (opcode) {
      case Opcode::ADD:
        result = a + b;
        break;
      case Opcode::SUB:
        result = a - b;
        break;
      case Opcode::MULT:
        result = a * b;
        break;
      case Opcode::DIV:
        result = a / b;
        break;
      default:
        // unreachable
        break;
      }

      // push back to stack
      this->stack.push(result);
    } catch (const std::bad_variant_access &ex) {
      throw std::runtime_error(std::format("type mismatch"));
    }

    break;
  }

  default:
    throw std::runtime_error(
        std::format("invalid opcode: {:} (full instruction: {:#x})",
                    opcode_name(opcode), instruction));
  }

  this->pc_inc();
}

void VM::reset() {
  this->memory.fill(0x00);
  this->pc = 0;
  this->ir = 0;

  while (!this->stack.empty())
    this->stack.pop();

  this->kv.clear();
}

void VM::pc_inc() { this->pc += 4; }

void VM::load_program(std::vector<uint8_t> program) {
  // ensure program is not too long
  if (program.size() > MAX_PROGRAM_SIZE)
    throw std::length_error(std::format(
        "program too long (must be up to {} bytes)", MAX_PROGRAM_SIZE));

  for (int i = 0; i < std::size(program); i++) {
    this->memory[i] = program[i];
  }
}
