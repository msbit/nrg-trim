#include <stdexcept>

#include <sys/errno.h>

#include "errno_error.h"

errno_error::errno_error(const char *what_arg) : std::runtime_error(what_arg) {
  value = errno;
}
