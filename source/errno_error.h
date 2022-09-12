#ifndef __ERRNO_ERROR_H__
#define __ERRNO_ERROR_H__

#include <stdexcept>

struct errno_error : std::runtime_error {
  int value;

  errno_error(const char *);
};

#endif
