#include "../vm.hpp"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <format>
#include <fstream>
#include <iostream>
#include <iterator>
#include <ostream>

std::string read_stream(std::istream &stream) {
  return std::string(std::istreambuf_iterator<char>(stream), {});
}

void emit_uint32(std::ostream &stream, uint32_t val) {
  stream.put((val & 0xFF000000) >> 24);
  stream.put((val & 0x00FF0000) >> 16);
  stream.put((val & 0x0000FF00) >> 8);
  stream.put(val & 0x000000FF);
}

void emit_pushc(std::ostream &stream, uint32_t val) {
  emit_uint32(stream, (OP_PUSHC << 24) | (val & 0x00FFFFFF));
}

void emit_pushv(std::ostream &stream, uint32_t name) {
  emit_uint32(stream, (OP_PUSHV << 24) | (name & 0x00FFFFFF));
}

void emit_store(std::ostream &stream, uint32_t name) {
  emit_uint32(stream, (OP_STORE << 24) | (name & 0x00FFFFFF));
}

void emit_add(std::ostream &stream) { emit_uint32(stream, (OP_ADD << 24)); }
void emit_sub(std::ostream &stream) { emit_uint32(stream, (OP_SUB << 24)); }
void emit_mult(std::ostream &stream) { emit_uint32(stream, (OP_MULT << 24)); }
void emit_div(std::ostream &stream) { emit_uint32(stream, (OP_DIV << 24)); }

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

  return EXIT_SUCCESS;
}
