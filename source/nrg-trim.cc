#include <cstdint>
#include <cstdio>

#include <algorithm>
#include <fstream>

#include "nrg_file.h"

constexpr int64_t chunksize = 1024 * 1024;

void trim(const std::string &);

std::string without_extension(const std::string &);

int main(int argc, char **argv) {
  for (auto i = 1; i < argc; i++) {
    trim(argv[i]);
  }
}

void trim(const std::string &input) {
  std::ifstream in(input);

  auto v = get_version(in);
  if (v == nrg_version::none) {
    printf("%s: not an NRG file\n", input.c_str());
    return;
  }

  if (input.size() > 1024) {
    fprintf(stderr, "filename too big\n");
    return;
  }

  auto count = get_offset(in, v);

  std::ofstream out(without_extension(input) + ".iso");

  std::unique_ptr<char[]> chunk(new char[chunksize]);
  while (count > 0) {
    in.read(chunk.get(), std::min(chunksize, count));
    out.write(chunk.get(), in.gcount());
    count -= in.gcount();
  }
}

std::string without_extension(const std::string &input) {
  auto dotpos = input.find_last_of('.');
  if (dotpos == std::string::npos || dotpos == 0) {
    // no '.' character in filename, or '.' is first character, copy it all
    return input;
  }

  auto slashpos = input.find_last_of('/');
  if (slashpos != std::string::npos && dotpos < slashpos) {
    // filename contains '/' in position after '.', copy it all
    return input;
  }

  if (slashpos != std::string::npos && (dotpos - slashpos) == 1) {
    // filename contains '/.' for last slash and dot positions, copy it all
    return input;
  }

  return input.substr(0, dotpos);
}
