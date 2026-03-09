#include "../vm.hpp"
#include <cstdint>
#include <cstdio>
#include <format>
#include <iostream>
#include <stdexcept>
#include <variant>
#include <vector>

template <class... Ts> struct overloaded : Ts... {
  using Ts::operator()...;
};

std::vector<uint8_t> read_stream(std::istream &stream) {
  return std::vector<uint8_t>(std::istreambuf_iterator<char>(stream), {});
}

void dump_stack(VM &vm) {
  std::cout << "[ ";
  while (!vm.stack.empty()) {
    std::visit(overloaded{[](uint32_t val) { std::cout << val; },
                          [](bool val) { std::cout << val; },
                          [](std::string val) { std::cout << val; }},
               vm.stack.top());

    vm.stack.pop();
  }
  std::cout << "]";
}

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cerr << std::format("usage: {} [FILE]\n", argv[0]);
    return EXIT_FAILURE;
  }

  std::vector<uint8_t> bytes;

  if (argv[1][0] == '-') {
    bytes = read_stream(std::cin);
  } else {
    // std::ifstream stream(argv[1]);
    // bytes = read_stream(stream);
  }

  VM vm = VM();
  vm.load_program(bytes);

  for (;;) {

    try {
      vm.fetch();
    } catch (const std::runtime_error &ex) {
      break;
    }

    vm.execute();
  }

  // for (int i = 0; i < vm.memory_size(); i++)
  //   printf("%x: %x\n", i, vm[i]);

  for (auto pair : vm.kv) {
    std::cout << pair.first << ": ";

    std::visit(overloaded{[](uint32_t val) { std::cout << val; },
                          [](bool val) { std::cout << val; },
                          [](std::string val) { std::cout << val; }},
               pair.second);

    std::cout << std::endl;
  }

  return 0;
}
