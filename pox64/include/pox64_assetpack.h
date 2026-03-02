#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>

namespace pox64 {

struct AssetEntry {
  uint64_t nameHash = 0;
  uint64_t offset = 0;
  uint64_t length = 0;
  uint32_t type = 0;
  uint32_t compression = 0;
};

class AssetPack {
public:
  bool load(const std::string& path);
  bool get(uint64_t nameHash, std::vector<uint8_t>& out) const;
  bool get_by_name(const std::string& name, std::vector<uint8_t>& out) const;
  
  static uint64_t hash_name(const std::string& s);

private:
  std::vector<uint8_t> m_blob;
  std::unordered_map<uint64_t, AssetEntry> m_index;
};

}
