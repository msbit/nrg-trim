#include <cstdint>
#include <cstdio>

#include <fstream>

#include "nrg_file.h"

void info(const char *);

int main(int argc, char **argv) {
  for (auto i = 1; i < argc; i++) {
    info(argv[i]);
  }
}

void print_hex(const uint8_t *buffer, int size) {
  for (auto i = 0; i < size; i++) {
    printf("%02x", buffer[i]);
    if ((i + 1) % 16 == 0) {
      printf("\n");
    }
  }
}

void info(const char *filename) {
  std::ifstream f(filename);
  f.seekg(0, std::ios::end);
  auto size = f.tellg();
  f.seekg(0, std::ios::beg);

  auto v = get_version(f);
  if (v == nrg_version::none) {
    printf("%s: not an NRG file\n", filename);
    return;
  }

  auto offset = get_offset(f, v);
  printf("%lld\n", offset);

  f.seekg(offset, std::ios::beg);
  auto footer_size = size - offset;
  uint8_t *footer = (uint8_t *)std::calloc(footer_size, 1);
  f.read(reinterpret_cast<char *>(footer), footer_size);
  print_hex(footer, footer_size);
}
