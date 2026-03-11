#include <cstdio>
#include <cstdlib>
#include <format>
#include <iostream>
#include <vector>

std::vector<uint8_t> read_stream(std::istream &stream) {
  return std::vector<uint8_t>(std::istreambuf_iterator<char>(stream), {});
}

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cerr << std::format("usage: {} [FILE]\n", argv[0]);
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
