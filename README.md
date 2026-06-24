# RISC-V Assembler
 
A two-pass assembler written in C++ that converts RISC-V assembly (`.asm`) into 32-bit binary machine code. Supports the full RV32I base integer instruction set (40 instructions).
 
---
 
## What it does
 
Takes a `.asm` file written in RISC-V assembly and produces an output file where each line is the 32-bit binary encoding of one instruction.
 
```
factorial.asm  →  [assembler]  →  factorial.out
```
 
Example — the instruction `ADDI x1, x0, 5` becomes:
 
```
00000000010100000000000010010011
```
 
---
 
## Supported instructions
 
All 40 instructions from the RV32I base integer ISA:
 
| Format | Instructions |
|--------|-------------|
| R-type | ADD, SUB, SLL, SLT, SLTU, XOR, SRL, SRA, OR, AND |
| I-type | ADDI, SLTI, SLTIU, XORI, ORI, ANDI, SLLI, SRLI, SRAI |
| I-type (loads) | LB, LH, LW, LBU, LHU, JALR |
| S-type | SB, SH, SW |
| B-type | BEQ, BNE, BLT, BGE, BLTU, BGEU |
| U-type | LUI, AUIPC |
| J-type | JAL |
| System | FENCE, ECALL, EBREAK |
 
---
 
## Build
 
Requires `g++` with C++17 support and `make`.
 
```bash
make
```
 
This produces an executable called `assembler` in the project root.
 
To clean up:
 
```bash
make clean
```
 
---
 
## Usage
 
```bash
./assembler <input.asm> <output.bin>
```
 
Example:
 
```bash
./assembler asm/factorial.asm factorial.out
./assembler asm/fibonacci.asm fibonacci.out
```
 
Or using the Makefile shortcut:
 
```bash
make run FILE=asm/factorial.asm
```
 
Output is written one instruction per line as a 32-character binary string.
 
---
 
## Assembly syntax
 
Standard RISC-V assembly. Registers can be written as `x0`–`x31` or using ABI names (`zero`, `ra`, `t0`, `a0`, etc.).
 
```asm
# Load n=5 into t0
ADDI t0, zero, 5
 
# Iterative loop
loop:
    ADD  t1, t1, t2
    ADDI t0, t0, -1
    BNE  t0, zero, loop
 
# Store result
ADDI a0, t1, 0
```
 
Comments start with `#`. Labels end with `:`. A label can be on its own line or on the same line as an instruction.
 
Load and store instructions use `offset(register)` syntax:
 
```asm
LW  x1, 4(x2)    # load word from address (x2 + 4)
SW  x1, 8(x3)    # store word to address (x3 + 8)
```
 
---
 
## How it works — two-pass design
 
A single-pass assembler cannot resolve forward references — if a branch instruction references a label that appears later in the file, the address isn't known yet. Two passes solve this.
 
**Pass 1 — Build the symbol table**
 
Scans every line and records the byte address of each label into a `std::map<string, int>`. No encoding happens here.
 
**Pass 2 — Encode instructions**
 
Walks every line again. For each instruction, looks up operands and (for branches/jumps) resolves label addresses from the symbol table. Encodes each instruction into its 32-bit binary representation using the format rules from the RISC-V spec.
 
---
 
## Project structure
 
```
assembler/
├── main.cpp                  # Entry point — orchestrates the two passes
├── Makefile
├── asm/
│   ├── factorial.asm         # Test program: iterative factorial
│   └── fibonacci.asm         # Test program: iterative Fibonacci
└── src/
    ├── InstructionTable.h    # Lookup table for all 40 RV32I instructions
    ├── Lexer.h / .cpp        # Tokenises each line of the .asm file
    ├── SymbolTable.h / .cpp  # Stores label → address mappings
    ├── FirstPass.h / .cpp    # Scans for labels, builds symbol table
    ├── Encoder.h / .cpp      # Encodes one instruction → uint32_t
    └── SecondPass.h / .cpp   # Encodes all instructions, produces output
```
 
---
 
## Verifying output
 
Output can be verified against [RARS](https://github.com/TheThirdOne/rars) (RISC-V Assembler and Runtime Simulator). Load the same `.asm` file in RARS, assemble it, and compare the machine code shown in the Text Segment panel against this assembler's output. They should match exactly.
 
---
 
## Error handling
 
The assembler exits with a message on:
- Unknown instruction mnemonic
- Undefined label referenced in a branch or jump
- Invalid register name
- Immediate value out of range for the instruction format
- Input file not found