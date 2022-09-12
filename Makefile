CXXFLAGS := -Wall -Wextra
CXXFLAGS += -std=c++11

LDLIBS := -lc++

build/%.o: source/%.cc
	mkdir -p build
	$(COMPILE.cc) $(OUTPUT_OPTION) $<

build/nrg-trim: build/nrg-trim.o build/errno_error.o build/file.o build/nrg_file.o

build/errno_error.o: source/errno_error.cc source/errno_error.h
build/errno_error.o: source/file.h

build/file.o: source/file.cc source/file.h
build/file.o: source/errno_error.h

build/nrg_file.o: source/nrg_file.cc source/nrg_file.h
build/nrg_file.o: source/file.h

build/nrg-trim.o: source/nrg-trim.cc
build/nrg-trim.o: source/errno_error.h source/file.h source/nrg_file.h

clean:
	rm -rf build
