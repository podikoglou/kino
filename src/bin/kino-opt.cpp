#include "../value.hpp"
#include "../vm.hpp"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <format>
#include <iostream>
#include <unordered_map>
#include <vector>

constexpr uint64_t FNV_OFFSET_BASIS = 0xcbf29ce484222325;
constexpr uint64_t FNV_PRIME = 0x100000001b3;

template <class... Ts> struct overloaded : Ts... {
  using Ts::operator()...;
};

std::vector<uint8_t> read_stream(std::istream &stream) {
  return std::vector<uint8_t>(std::istreambuf_iterator<char>(stream), {});
}

uint64_t hash_byte(uint64_t hash, uint8_t byte) {
  hash = hash ^ byte;
  hash *= FNV_PRIME;
  return hash;
}

uint64_t hash_kv(const std::unordered_map<uint64_t, value> &kv) {
  uint64_t hash = 0x00;

  for (auto pair : kv) {
    // hash key
    uint64_t key = pair.first;

    for (int i = 0; i < 8; i++) {
      hash = hash_byte(hash, key & 0xFF);
      key >>= 8;
    }

    // hash value
    auto value = pair.second;

    std::visit(overloaded{[hash](uint32_t val) mutable {
                            {
                              // hash uint32_t
                              for (int i = 0; i < 4; i++) {
                                hash = hash_byte(hash, val & 0xFF);
                                val >>= 8;
                              }
                            }
                          },
                          [hash](bool val) mutable {
                            // hash bool
                            hash = hash_byte(hash, val ? 0x01 : 0x00);
                          },
                          [hash](std::string val) mutable {
                            // hash string
                            for (char c : val) {
                              hash = hash_byte(hash, c);
                            }
                          }},
               value);
  }

  return hash;
}

uint64_t hash_program_output(VM &vm, const std::vector<uint8_t> program) {
  vm.reset();
  vm.load_program(program);

  for (;;) {

    try {
      vm.fetch();
    } catch (const std::runtime_error &ex) {
      break;
    }

    vm.execute();
  }

  return hash_kv(vm.kv);
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
  uint64_t output_hash = hash_program_output(vm, bytes);

  printf("program output hash: %lx\n", output_hash);

  return EXIT_SUCCESS;
}
