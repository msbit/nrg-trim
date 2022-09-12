#ifndef __NRG_FILE_H__
#define __NRG_FILE_H__

#include <cstdint>

#include <fstream>

enum struct nrg_version { none, v1, v2 };

nrg_version get_version(std::ifstream &);

int64_t get_offset(std::ifstream &, nrg_version);

#endif
