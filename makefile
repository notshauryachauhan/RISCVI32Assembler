CXX     = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g

# All .cpp files in src/ + main.cpp
SRCS = main.cpp \
       src/Lexer.cpp \
       src/SymbolTable.cpp \
       src/Encoder.cpp \
       src/FirstPass.cpp \
       src/SecondPass.cpp

TARGET = assembler

# Default target — build the assembler
all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

# Run with a specific asm file: make run FILE=asm/factorial.asm
run: $(TARGET)
	./$(TARGET) $(FILE) output.bin

# Remove compiled binary
clean:
	rm -f $(TARGET) output.bin

.PHONY: all run clean