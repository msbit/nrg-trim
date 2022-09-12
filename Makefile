BUILD_DIR ?= build
SOURCE_DIR ?= source

CXXFLAGS := -Wall -Wextra
CXXFLAGS += -std=c++11

LDLIBS := -lc++

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cc
	mkdir -p $(BUILD_DIR)
	$(COMPILE.cc) $(OUTPUT_OPTION) $<

$(BUILD_DIR)/nrg-trim: $(BUILD_DIR)/nrg-trim.o $(BUILD_DIR)/errno_error.o $(BUILD_DIR)/file.o $(BUILD_DIR)/nrg_file.o

$(BUILD_DIR)/errno_error.o: $(SOURCE_DIR)/errno_error.cc $(SOURCE_DIR)/errno_error.h
$(BUILD_DIR)/errno_error.o: $(SOURCE_DIR)/file.h

$(BUILD_DIR)/file.o: $(SOURCE_DIR)/file.cc $(SOURCE_DIR)/file.h
$(BUILD_DIR)/file.o: $(SOURCE_DIR)/errno_error.h

$(BUILD_DIR)/nrg_file.o: $(SOURCE_DIR)/nrg_file.cc $(SOURCE_DIR)/nrg_file.h
$(BUILD_DIR)/nrg_file.o: $(SOURCE_DIR)/file.h

$(BUILD_DIR)/nrg-trim.o: $(SOURCE_DIR)/nrg-trim.cc
$(BUILD_DIR)/nrg-trim.o: $(SOURCE_DIR)/errno_error.h $(SOURCE_DIR)/file.h $(SOURCE_DIR)/nrg_file.h

clean:
	rm -rf $(BUILD_DIR)
