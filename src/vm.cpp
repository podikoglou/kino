#include "vm.hpp"

VM::VM() {}

void VM::reset() { this->memory.fill(0x00); }
