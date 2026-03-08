#include "../vm.hpp"
#include <cstdio>

int main(void) {
  VM vm = VM();

  for (int i = 0; i < vm.memory_size(); i++)
    printf("%x: %x\n", i, vm[i]);

  return 0;
}
