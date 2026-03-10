#pragma once

#include <cstdint>
#include <string>
#include <variant>

typedef std::variant<uint32_t, bool, std::string> value;
