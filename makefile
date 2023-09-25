# Compiler and flags
CC := g++
CFLAGS := -Wall -Wextra -std=c++11
LDFLAGS := -L./lib
LDLIBS := -lglfw3 -lGL

# Directories
SRC_DIR := source
INCLUDE_DIR := include
BUILD_DIR := lib/build
OUTPUT_DIR := build

# Source files
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/*.c)

# Object files
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(filter %.cpp,$(SRC_FILES))) \
             $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(filter %.c,$(SRC_FILES)))

# Executable
TARGET := $(OUTPUT_DIR)/source.exe

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ_FILES) | $(OUTPUT_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -o $@ -c $<

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -o $@ -c $<

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(OUTPUT_DIR):
	mkdir -p $(OUTPUT_DIR)

clean:
	rm -rf $(BUILD_DIR) $(OUTPUT_DIR)
