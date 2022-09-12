#include <cstdint>
#include <cstdio>

#include <algorithm>
#include <fstream>

#include "nrg_file.h"

constexpr int64_t chunksize = 1024 * 1024;

void trim(const char *);

int without_extension(const char *, char *, int);

int main(int argc, char **argv) {
  for (auto i = 1; i < argc; i++) {
    trim(argv[i]);
  }
}

void trim(const char *filename) {
  std::ifstream in(filename);

  auto v = get_version(in);
  if (v == nrg_version::none) {
    printf("%s: not an NRG file\n", filename);
    return;
  }

  auto length = strlen(filename);
  if (length > 1024) {
    fprintf(stderr, "filename too big\n");
    return;
  }

  // enough space for original + '.iso\0'
  std::unique_ptr<char[]> output(new char[length + 5]);
  length = without_extension(filename, output.get(), length);
  memcpy(output.get() + length, ".iso", 5);

  auto count = get_offset(in, v);

  std::ofstream out(output.get());

  in.seekg(0, std::ios::beg);
  std::unique_ptr<char[]> chunk(new char[chunksize]);
  while (count > 0) {
    in.read(chunk.get(), std::min(chunksize, count));
    out.write(chunk.get(), in.gcount());
    count -= in.gcount();
  }
}

int without_extension(const char *input, char *output, int length) {
  auto dotpos = strrchr(input, '.');
  if (dotpos == nullptr || dotpos == input) {
    // no '.' character in filename, or '.' is first character, copy it all
    memcpy(output, input, length);
    output[length] = '\0';
    return length;
  }

  auto slashpos = strrchr(input, '/');
  if (slashpos != nullptr && dotpos < slashpos) {
    // filename contains '/' in position after '.', copy it all
    memcpy(output, input, length);
    output[length] = '\0';
    return length;
  }

  if (slashpos != nullptr && (dotpos - slashpos) == 1) {
    // filename contains '/.' for last slash and dot positions, copy it all
    memcpy(output, input, length);
    output[length] = '\0';
    return length;
  }

  length = dotpos - input;
  memcpy(output, input, length);
  output[length] = '\0';
  return length;
}
