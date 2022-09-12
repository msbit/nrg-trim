#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <stdexcept>

#include <sys/errno.h>

#include "errno_error.h"
#include "file.h"
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
  file f(filename);
  auto v = get_version(f);
  if (v == nrg_version::none) {
    printf("%s: not an NRG file\n", filename);
    return;
  }

  auto offset = get_offset(f, v);
  printf("%lld\n", offset);

  f.seek(offset, SEEK_SET);
  auto footer_size = f.size - offset;
  uint8_t *footer = (uint8_t *)std::calloc(footer_size, 1);
  f.read(footer, 1, footer_size);
  print_hex(footer, footer_size);
}
