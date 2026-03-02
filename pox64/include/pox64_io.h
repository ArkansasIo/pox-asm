#pragma once
#include <vector>
#include <string>
#include <cstdint>

namespace pox64 {
  bool read_file(const std::string& path, std::vector<uint8_t>& out);
  bool write_file(const std::string& path, const std::vector<uint8_t>& data);
}
