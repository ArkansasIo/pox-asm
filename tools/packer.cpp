#include "pox_endian.h"
#include "pox_crc16.h"
#include <filesystem>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>

static constexpr uint32_t MAGIC = 0x4B415850;
static constexpr uint16_t VER = 1;

static uint64_t hash_name(const std::string& s) {
  uint64_t h = 14695981039346656037ull;
  for (unsigned char c : s) { h ^= (uint64_t)c; h *= 1099511628211ull; }
  return h;
}

static bool read_file(const std::string& path, std::vector<uint8_t>& out) {
  std::ifstream f(path, std::ios::binary);
  if (!f) return false;
  f.seekg(0, std::ios::end);
  auto n = (size_t)f.tellg();
  f.seekg(0, std::ios::beg);
  out.resize(n);
  if (n) f.read((char*)out.data(), (std::streamsize)n);
  return true;
}
static bool write_file(const std::string& path, const std::vector<uint8_t>& data) {
  std::ofstream f(path, std::ios::binary);
  if (!f) return false;
  if (!data.empty()) f.write((const char*)data.data(), (std::streamsize)data.size());
  return true;
}

int main(int argc, char** argv) {
  if (argc < 3) {
    std::cerr << "Usage: pox_packer <input_folder> <out.pak>\n";
    return 2;
  }
  std::filesystem::path in = argv[1];
  std::string outPath = argv[2];
  
  struct EntryTmp { std::string name; uint64_t hash; std::vector<uint8_t> data; };
  
  std::vector<EntryTmp> entries;
  for (auto& p : std::filesystem::directory_iterator(in)) {
    if (!p.is_regular_file()) continue;
    auto name = p.path().filename().string();
    std::vector<uint8_t> blob;
    if (!read_file(p.path().string(), blob)) continue;
    entries.push_back({name, hash_name(name), std::move(blob)});
    std::cout << "Add " << name << " (" << entries.back().data.size() << " bytes)\n";
  }
  if (entries.empty()) {
    std::cerr << "No files in input folder.\n";
    return 3;
  }
  
  std::vector<uint8_t> pak;
  pak.resize(24);
  
  uint64_t cursor = 24;
  struct Dir { uint64_t h, off, len; uint32_t type, comp; };
  std::vector<Dir> dir;
  dir.reserve(entries.size());
  
  for (auto& e : entries) {
    uint64_t off = cursor;
    uint64_t len = (uint64_t)e.data.size();
    pak.insert(pak.end(), e.data.begin(), e.data.end());
    cursor += len;
    
    Dir d{ e.hash, off, len, 0u, 0u };
    dir.push_back(d);
  }
  
  uint64_t dirOffset = cursor;
  for (auto& d : dir) {
    uint8_t ent[32];
    pox_wrle64(ent+0, d.h);
    pox_wrle64(ent+8, d.off);
    pox_wrle64(ent+16, d.len);
    pox_wrle32(ent+24, d.type);
    pox_wrle32(ent+28, d.comp);
    pak.insert(pak.end(), ent, ent+32);
  }
  
  pox_wrle32(pak.data()+0, MAGIC);
  pox_wrle16(pak.data()+4, VER);
  pox_wrle16(pak.data()+6, (uint16_t)dir.size());
  pox_wrle64(pak.data()+8, dirOffset);
  pox_wrle64(pak.data()+16, 0);
  
  if (!write_file(outPath, pak)) {
    std::cerr << "Failed to write " << outPath << "\n";
    return 4;
  }
  std::cout << "Wrote " << outPath << " size=" << pak.size() << "\n";
  return 0;
}
