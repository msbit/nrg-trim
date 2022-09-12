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

template <typename T> struct allocated {
  T *ptr;

  allocated(size_t size) : ptr(static_cast<T *>(malloc(size))) {
    if (ptr == nullptr) {
      throw std::runtime_error("malloc");
    }
  }

  ~allocated() { free(ptr); }
};

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
  allocated<char> output(length + 5);
  length = without_extension(filename, output.ptr, length);
  memcpy(output.ptr + length, ".iso", 5);

  auto count = get_offset(in, v);

  std::ofstream out(output.ptr);

  in.seekg(0, std::ios::beg);
  allocated<char> chunk(chunksize);
  while (count > 0) {
    in.read(chunk.ptr, std::min(chunksize, count));
    out.write(chunk.ptr, in.gcount());
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
