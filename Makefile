CXX := g++
CPPFLAGS := -Iinclude
CXXFLAGS := -std=c++20 -Wall -Wextra -Wpedantic -Wshadow
BUILD_DIR := build
APP_SOURCES := src/book.cpp src/user.cpp src/library.cpp src/main.cpp
APP_OBJECTS := $(APP_SOURCES:src/%.cpp=$(BUILD_DIR)/%.o)
TEST_OBJECTS := $(BUILD_DIR)/book.o $(BUILD_DIR)/user.o $(BUILD_DIR)/library.o $(BUILD_DIR)/library_tests.o
TARGET := $(BUILD_DIR)/library_project
TEST_TARGET := $(BUILD_DIR)/library_tests

.PHONY: all run test clean
all: $(TARGET)
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
$(BUILD_DIR)/%.o: src/%.cpp | $(BUILD_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@
$(BUILD_DIR)/library_tests.o: tests/library_tests.cpp include/book.h include/user.h include/library.h | $(BUILD_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@
$(TARGET): $(APP_OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@
$(TEST_TARGET): $(TEST_OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@
run: $(TARGET)
	./$(TARGET)
test: $(TEST_TARGET)
	./$(TEST_TARGET)
clean:
	rm -rf $(BUILD_DIR) bin obj
