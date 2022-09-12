CXXFLAGS := -Wall -Wextra
CXXFLAGS += -std=c++11

LDLIBS := -lc++

nrg-trim: nrg-trim.o errno_error.o file.o nrg_file.o

errno_error.o: errno_error.cc errno_error.h
errno_error.o: file.h

file.o: file.cc file.h
file.o: errno_error.h

nrg_file.o: nrg_file.cc nrg_file.h
nrg_file.o: file.h

nrg-trim.o: nrg-trim.cc
nrg-trim.o: errno_error.h file.h nrg_file.h

clean:
	rm -f nrg-trim errno_error.o file.o nrg-trim.o nrg_file.o
