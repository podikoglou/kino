#include "../isa.hpp"
#include <cstdint>
#include <cstdlib>
#include <format>
#include <iostream>
#include <iterator>

struct InstructionIterator {
  using iterator_category = std::input_iterator_tag;
  using difference_type = std::ptrdiff_t;
  using value_type = Instruction;
  using pointer = void;
  using reference = Instruction;

private:
  Opcode opcode;

  // 0000000 -> 00FFFFFF
  uint32_t operand;

public:
  InstructionIterator() : opcode{Opcode::PUSHC}, operand(0x00) {}

  value_type operator*() const {
    uint32_t op = static_cast<uint32_t>(this->opcode);

    return static_cast<Instruction>(op << 24) | (operand & 0x00FFFFFF);
  }

  InstructionIterator operator++(int) {
    InstructionIterator tmp = *this;
    ++(*this);
    return tmp;
  }

  InstructionIterator &operator++() {
    switch (this->opcode) {
    case Opcode::PUSHC: {

      if (this->operand == 0x00FFFFFF) {
        this->opcode = Opcode::PUSHV;
        this->operand = 0;
      } else {
        this->operand++;
      }
      break;
    }

    case Opcode::PUSHV: {

      if (this->operand == 0x00FFFFFF) {
        this->opcode = Opcode::STORE;
        this->operand = 0;
      } else {
        this->operand++;
      }
      break;
    }

    case Opcode::STORE: {

      if (this->operand == 0x00FFFFFF) {
        this->opcode = Opcode::ADD;
        this->operand = 0;
      } else {
        this->operand++;
      }
      break;
    }

    case Opcode::ADD: {
      this->opcode = Opcode::SUB;
      this->operand = 0;
      break;
    }

    case Opcode::SUB: {
      this->opcode = Opcode::MULT;
      this->operand = 0;
      break;
    }

    case Opcode::MULT: {
      this->opcode = Opcode::DIV;
      this->operand = 0;
      break;
    }

    case Opcode::DIV: {
      this->operand = 1;
      break;
    }
    }

    return *this;
  }

  static InstructionIterator begin() {
    InstructionIterator it;
    it.opcode = Opcode::PUSHC;
    it.operand = 0;
    return it;
  }

  static InstructionIterator end() {
    InstructionIterator it;
    it.opcode = Opcode::DIV;
    it.operand = 1;
    return it;
  }

  bool operator==(const InstructionIterator other) const {
    return this->opcode == other.opcode && this->operand == other.operand;
  }
};

struct InstructionRange {
  auto begin() { return InstructionIterator::begin(); }
  auto end() { return InstructionIterator::end(); }
};

int main(int argc, char **argv) {
  // i dont actually know what these do but they speed up io a lot
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(NULL);

  for (auto i : InstructionRange())
    std::cout << std::format("{:#x}\n", i);

  return EXIT_SUCCESS;
}
