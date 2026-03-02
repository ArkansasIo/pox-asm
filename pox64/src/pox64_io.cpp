#include "pox64_io.h"
#include <fstream>

namespace pox64 {
  bool read_file(const std::string& path, std::vector<uint8_t>& out) {
    std::ifstream f(path, std::ios::binary);
    if (!f) return false;
    f.seekg(0, std::ios::end);
    auto n = (size_t)f.tellg();
    f.seekg(0, std::ios::beg);
    out.resize(n);
    if (n) f.read((char*)out.data(), (std::streamsize)n);
    return true;
  }
  
  bool write_file(const std::string& path, const std::vector<uint8_t>& data) {
    std::ofstream f(path, std::ios::binary);
    if (!f) return false;
    if (!data.empty()) f.write((const char*)data.data(), (std::streamsize)data.size());
    return true;
  }
}
