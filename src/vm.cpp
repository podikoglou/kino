#include "vm.hpp"

VM::VM() { this->reset(); }

void VM::reset() { this->memory.fill(0x00); }


const size_t VM::memory_size() { return MEMORY_SIZE; }
