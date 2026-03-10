#include "../isa.hpp"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <format>
#include <fstream>
#include <iostream>
#include <iterator>
#include <ostream>

// TODO:
// - lex assembly
// - create ast structures
// - parse into ast
// - preprocess
//    - implement FNV-1 and hash variable names

#define EMITTER(name, params...) void name(std::ostream &stream, params)

#define EMITTER_0(name, opcode)                                                \
  void name(std::ostream &stream) { emit_uint32(stream, (opcode << 24)); }

std::string read_stream(std::istream &stream) {
  return std::string(std::istreambuf_iterator<char>(stream), {});
}

EMITTER(emit_uint32, uint32_t val) {
  stream.put((val & 0xFF000000) >> 24);
  stream.put((val & 0x00FF0000) >> 16);
  stream.put((val & 0x0000FF00) >> 8);
  stream.put(val & 0x000000FF);
}

EMITTER(emit_pushc, uint32_t val) {
  emit_uint32(stream, (OP_PUSHC << 24) | (val & 0x00FFFFFF));
}

EMITTER(emit_pushv, uint32_t name) {
  emit_uint32(stream, (OP_PUSHV << 24) | (name & 0x00FFFFFF));
}

EMITTER(emit_store, uint32_t name) {
  emit_uint32(stream, (OP_STORE << 24) | (name & 0x00FFFFFF));
}

EMITTER_0(emit_add, OP_ADD);
EMITTER_0(emit_sub, OP_SUB);
EMITTER_0(emit_mult, OP_MULT);
EMITTER_0(emit_div, OP_DIV);

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cerr << std::format("usage: {} [FILE]\n", argv[0]);
    return EXIT_FAILURE;
  }

  std::string contents;

  if (argv[1][0] == '-') {
    contents = read_stream(std::cin);
  } else {
    std::ifstream stream(argv[1]);
    contents = read_stream(stream);
  }

  emit_pushc(std::cout, 4);
  emit_store(std::cout, 49);

  emit_pushv(std::cout, 49);
  emit_pushv(std::cout, 49);
  emit_mult(std::cout);

  emit_store(std::cout, 50);

  return EXIT_SUCCESS;
}
