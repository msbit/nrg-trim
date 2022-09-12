#ifndef __FILE_H__
#define __FILE_H__

#include <cstdio>

#include "errno_error.h"

struct file final {
  FILE *f;
  off_t size;

  file(const char *);

  void seek(long, int);

  size_t read(void *, size_t, size_t);

  long tell();

  void rewind();

  ~file();
};

#endif
