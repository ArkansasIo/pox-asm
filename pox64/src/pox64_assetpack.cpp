#include "pox64_assetpack.h"
#include "pox64_io.h"
#include "pox64_log.h"
#include "pox_endian.h"

namespace pox64 {

static constexpr uint32_t MAGIC = 0x4B415850;

uint64_t AssetPack::hash_name(const std::string& s) {
  uint64_t h = 14695981039346656037ull;
  for (unsigned char c : s) { h ^= (uint64_t)c; h *= 1099511628211ull; }
  return h;
}

bool AssetPack::load(const std::string& path) {
  if (!read_file(path, m_blob)) {
    log_err("AssetPack: failed to read " + path);
    return false;
  }
  if (m_blob.size() < 24) return false;
  
  const uint8_t* p = m_blob.data();
  uint32_t magic = pox_le32(p); p += 4;
  if (magic != MAGIC) { log_err("AssetPack: bad magic"); return false; }
  uint16_t ver = pox_le16(p); p += 2;
  uint16_t count = pox_le16(p); p += 2;
  uint64_t dirOff = pox_le64(p); p += 8;
  (void)ver;
  
  if (dirOff + (uint64_t)count * 32ull > (uint64_t)m_blob.size()) {
    log_err("AssetPack: directory out of range");
    return false;
  }
  
  m_index.clear();
  const uint8_t* d = m_blob.data() + (size_t)dirOff;
  for (uint16_t i = 0; i < count; i++) {
    AssetEntry e;
    e.nameHash = pox_le64(d + 0);
    e.offset   = pox_le64(d + 8);
    e.length   = pox_le64(d + 16);
    e.type     = pox_le32(d + 24);
    e.compression = pox_le32(d + 28);
    d += 32;
    
    if (e.offset + e.length > (uint64_t)m_blob.size()) continue;
    m_index[e.nameHash] = e;
  }
  
  log_info("AssetPack: loaded " + std::to_string(m_index.size()) + " assets");
  return true;
}

bool AssetPack::get(uint64_t nameHash, std::vector<uint8_t>& out) const {
  auto it = m_index.find(nameHash);
  if (it == m_index.end()) return false;
  const AssetEntry& e = it->second;
  out.assign(m_blob.begin() + (size_t)e.offset, m_blob.begin() + (size_t)(e.offset + e.length));
  return true;
}

bool AssetPack::get_by_name(const std::string& name, std::vector<uint8_t>& out) const {
  return get(hash_name(name), out);
}

}
