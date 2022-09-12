CXXFLAGS := -Wall -Wextra
CXXFLAGS += -std=c++11

LDLIBS := -lc++

build/%.o: %.cc
	mkdir -p build
	$(COMPILE.cc) $(OUTPUT_OPTION) $<

build/nrg-trim: build/nrg-trim.o build/errno_error.o build/file.o build/nrg_file.o

build/errno_error.o: errno_error.cc errno_error.h
build/errno_error.o: file.h

build/file.o: file.cc file.h
build/file.o: errno_error.h

build/nrg_file.o: nrg_file.cc nrg_file.h
build/nrg_file.o: file.h

build/nrg-trim.o: nrg-trim.cc
build/nrg-trim.o: errno_error.h file.h nrg_file.h

clean:
	rm -rf build
