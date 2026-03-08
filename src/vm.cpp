#include "vm.hpp"

VM::VM() { this->reset(); }

void VM::reset() { this->memory.fill(0x00); }
