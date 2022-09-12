#include <cstdio>

#include <sys/errno.h>

#include "file.h"

#include "errno_error.h"

file::file(const char *filename) {
  f = std::fopen(filename, "r");
  if (f == nullptr) {
    throw errno_error("fopen");
  }

  seek(0, SEEK_END);
  size = tell();
  rewind();
}

void file::seek(long offset, int whence) {
  auto result = std::fseek(f, offset, whence);
  if (result != 0) {
    throw errno_error("fseek");
  }
}

size_t file::read(void *buffer, size_t size, size_t count) {
  auto result = std::fread(buffer, size, count, f);
  if (result == count) {
    return result;
  }

  if (feof(f)) {
    return result;
  }

  throw errno_error("fread");
}

long file::tell() {
  auto result = std::ftell(f);
  if (result == -1) {
    throw errno_error("ftell");
  }

  return result;
}

void file::rewind() {
  errno = 0;
  std::rewind(f);
  if (errno != 0) {
    throw errno_error("rewind");
  }
}

file::~file() { std::fclose(f); }
