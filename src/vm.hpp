#pragma once

#include <array>
#include <cstdint>

class VM {
private:
  std::array<uint8_t, 2048> memory;

public:
  VM();
};
