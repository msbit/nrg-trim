#include <cstdint>
#include <cstring>

#include <fstream>
#include <stdexcept>

#include "nrg_file.h"

struct rewinder final {
  std::istream &stream;
  rewinder(std::istream &stream) : stream(stream) {}
  ~rewinder() { stream.seekg(0, std::ios::beg); }
};

nrg_version get_version(std::ifstream &f) {
  rewinder r(f);

  f.seekg(-12, std::ios::end);
  uint8_t id[4];
  f.read(reinterpret_cast<char *>(id), 4);
  if (memcmp("NER5", id, 4) == 0) {
    return nrg_version::v2;
  }

  f.read(reinterpret_cast<char *>(id), 4);
  if (memcmp("NER5", id, 4) == 0) {
    return nrg_version::v1;
  }

  return nrg_version::none;
}

int64_t get_offset(std::ifstream &f, nrg_version v) {
  rewinder r(f);

  if (v == nrg_version::none) {
    throw std::invalid_argument("can't get offset for non-NRG file");
  }

  uint64_t offset;
  if (v == nrg_version::v1) {
    // TODO very iffy
    f.seekg(-4, std::ios::end);
    f.read(reinterpret_cast<char *>(&offset), 4);
  }

  if (v == nrg_version::v2) {
    f.seekg(-8, std::ios::end);
    f.read(reinterpret_cast<char *>(&offset), 8);
  }

  return ntohll(offset);
}
