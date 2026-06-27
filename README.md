# RISC-V Assembler

A two-pass assembler written in C++ that converts RISC-V assembly (`.asm`) into 32-bit binary machine code. Implements the full RV32I base integer instruction set (40 instructions).

---

## What it does

Takes a `.asm` file written in RISC-V assembly and produces an output file where each line is the 32-bit binary encoding of one instruction.

```
factorial.asm  →  [assembler]  →  factorial.out
```

For example, `ADDI x1, x0, 5` becomes:

```
00000000010100000000000010010011
```

---

## Supported instructions

All 40 instructions from the RV32I base integer ISA:

| Format | Instructions |
|--------|-------------|
| R-type | ADD, SUB, SLL, SLT, SLTU, XOR, SRL, SRA, OR, AND |
| I-type (arithmetic) | ADDI, SLTI, SLTIU, XORI, ORI, ANDI, SLLI, SRLI, SRAI |
| I-type (loads) | LB, LH, LW, LBU, LHU, JALR |
| S-type | SB, SH, SW |
| B-type | BEQ, BNE, BLT, BGE, BLTU, BGEU |
| U-type | LUI, AUIPC |
| J-type | JAL |
| System | FENCE, ECALL, EBREAK |

Register names can be written as `x0`–`x31` or ABI names (`zero`, `ra`, `sp`, `t0`, `a0`, etc.) — both are supported.

---

## Project structure

```
RISCVI32Assembler/
├── main.cpp              # Entry point — orchestrates both passes, writes output
├── makefile
├── include/
│   ├── InstructionTable.h  # Lookup table for all 40 RV32I instructions (opcode, funct3, funct7, format)
│   ├── Lexer.h             # Tokeniser interface
│   ├── SymbolTable.h       # Label → address map interface
│   ├── FirstPass.h         # Label scanning interface
│   ├── Encoder.h           # Instruction encoder interface
│   └── SecondPass.h        # Encoding pass interface
├── src/
│   ├── Lexer.cpp           # Reads .asm file, strips comments, tokenises each line
│   ├── SymbolTable.cpp     # Wraps std::map<string,int> with error handling
│   ├── FirstPass.cpp       # Pass 1 — scans for labels, builds symbol table
│   ├── Encoder.cpp         # Encodes one instruction → uint32_t (all 6 formats)
│   └── SecondPass.cpp      # Pass 2 — encodes all instructions, returns vector<uint32_t>
└── asm/
    ├── factorial.asm       # Iterative factorial in RISC-V assembly
    └── fibonacci.asm       # Iterative Fibonacci in RISC-V assembly
```

---

## How it works — two-pass design

A single-pass assembler cannot resolve forward references. If a branch instruction like `BEQ x1, x2, end` references a label `end` that appears later in the file, the target address isn't known yet when the instruction is being encoded. Two passes solve this cleanly.

**Pass 1 — build the symbol table**

Scans every line and keeps a running address counter (increments by 4 per instruction since all RV32I instructions are 4 bytes). When a label is found (token ending in `:`), it records `label → currentAddress` in a `std::map<string, int>`. No encoding happens here.

**Pass 2 — encode instructions**

Walks every line again. For each instruction, extracts operands and calls the appropriate encoder for its format type (R, I, S, B, U, J). For branch and jump instructions, the target address is resolved from the symbol table and the PC-relative offset is computed as `targetAddress - currentAddress`. The result of each encoding is a `uint32_t` written to the output file as a 32-character binary string using `std::bitset<32>`.

---

## Build

Requires `g++` with C++17 support and `make`.

```bash
make
```

Produces an executable called `assembler` in the project root.

```bash
make clean    # remove compiled binary and output files
```

**On Windows:** use MSYS2 MinGW64 terminal or Git Bash with MinGW. The `-Iinclude` flag in the makefile handles header search paths automatically.

---

## Usage

```bash
./assembler <input.asm> <output.bin>
```

Examples:

```bash
./assembler asm/factorial.asm factorial.out
./assembler asm/fibonacci.asm fibonacci.out
```

Makefile shortcut:

```bash
make run FILE=asm/factorial.asm
```

Output is one instruction per line as a 32-character binary string.

---

## Assembly syntax

Standard RISC-V assembly. Comments start with `#`. Labels end with `:` and can appear on their own line or on the same line as an instruction. Mnemonics are case-insensitive.

```asm
# Iterative loop example
ADDI t0, zero, 5      # t0 = 5 (loop counter)
ADDI t1, zero, 1      # t1 = 1 (result starts at 1)

loop:
    ADD  t1, t1, t0   # result = result + counter  
    ADDI t0, t0, -1   # counter--
    BNE  t0, zero, loop

ADDI a0, t1, 0        # move result to a0
```

Load and store instructions use `offset(register)` syntax:

```asm
LW  x1, 4(x2)    # load word from address (x2 + 4) into x1
SW  x1, 8(x3)    # store word in x1 to address (x3 + 8)
```

---

## Verifying output

Assemble the same `.asm` file in [RARS](https://github.com/TheThirdOne/rars) (RISC-V Assembler and Runtime Simulator). In the Text Segment panel, RARS shows the machine code encoding of each instruction. The output from this assembler should match RARS exactly for every instruction.

---

## Error handling

The assembler exits with a descriptive message on:

- Unknown instruction mnemonic
- Undefined label referenced in a branch or jump
- Invalid register name
- Malformed memory operand (e.g. missing parentheses in `offset(reg)`)
- Input file not found or unreadable
- Output file cannot be opened for writing
