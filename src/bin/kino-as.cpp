#include <cstdio>
#include <cstdlib>
#include <format>
#include <fstream>
#include <iostream>
#include <iterator>

std::string read_stream(std::istream &stream) {
  return std::string(std::istreambuf_iterator<char>(stream), {});
}

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
