#include "isa.hpp"
#include <string_view>

const Opcode extract_opcode(Instruction instruction) {
  return static_cast<Opcode>((instruction & 0xFF000000) >> 24);
}

const std::string_view opcode_name(Opcode opcode) {
  switch (opcode) {
  case Opcode::PUSHC:
    return "PUSHC";
  case Opcode::PUSHV:
    return "PUSHV";
  case Opcode::STORE:
    return "STORE";
  case Opcode::ADD:
    return "ADD";
  case Opcode::SUB:
    return "SUB";
  case Opcode::MULT:
    return "MULT";
  case Opcode::DIV:
    return "DIV";
  };
}
