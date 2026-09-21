CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Iinclude
SRC := src/Date.cpp src/Book.cpp src/Member.cpp src/Loan.cpp src/Library.cpp
BIN_DIR := bin

.PHONY: all run test clean

all: $(BIN_DIR)/library_manager

$(BIN_DIR)/library_manager: src/main.cpp $(SRC) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BIN_DIR)/library_tests: tests/test_library.cpp $(SRC) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

run: all
	./$(BIN_DIR)/library_manager

test: $(BIN_DIR)/library_tests
	./$(BIN_DIR)/library_tests

clean:
	rm -rf $(BIN_DIR)
