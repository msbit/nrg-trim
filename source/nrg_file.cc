#include <cstring>

#include <stdexcept>

#include "nrg_file.h"

#include "file.h"

nrg_version get_version(file &f) {
  f.seek(-12, SEEK_END);
  uint8_t id[4];
  f.read(id, 1, 4);
  if (memcmp("NER5", id, 4) == 0) {
    return nrg_version::v2;
  }

  f.read(id, 1, 4);
  if (memcmp("NER5", id, 4) == 0) {
    return nrg_version::v1;
  }

  return nrg_version::none;
}

uint64_t get_offset(file &f, nrg_version v) {
  if (v == nrg_version::none) {
    throw std::invalid_argument("can't get offset for non-NRG file");
  }

  uint64_t offset;
  if (v == nrg_version::v1) {
    // TODO very iffy
    f.seek(-4, SEEK_END);
    f.read(&offset, 4, 1);
  }

  if (v == nrg_version::v2) {
    f.seek(-8, SEEK_END);
    f.read(&offset, 8, 1);
  }

  return ntohll(offset);
}
