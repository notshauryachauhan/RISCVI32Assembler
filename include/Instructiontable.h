#pragma once
#include <cstdint>
#include <string>

enum class Format {
    R,   // register-register  (ADD, SUB, AND, OR, XOR, SLL, SRL, SRA, SLT, SLTU)
    I,   // immediate + loads  (ADDI, LW, LH, LB, LHU, LBU, JALR, SLLI, SRLI, SRAI, ...)
    S,   // stores             (SW, SH, SB)
    B,   // branches           (BEQ, BNE, BLT, BGE, BLTU, BGEU)
    U,   // upper immediate    (LUI, AUIPC)
    J,   // jump               (JAL)
    SYSTEM  // FENCE, ECALL, EBREAK — handled as hardcoded values
};

struct InstructionInfo {
    std::string mnemonic;   // e.g. "ADD", "ADDI", "BEQ"
    Format      format;     // which encoding format to use
    uint32_t    opcode;     // 7-bit opcode  (bits [6:0])
    uint32_t    funct3;     // 3-bit funct3  (bits [14:12]) — 0 if unused
    uint32_t    funct7;     // 7-bit funct7  (bits [31:25]) — 0 if unused
};
 
static const InstructionInfo INSTRUCTION_TABLE[] = {
 
    // ── R-type ────────────────────────────────────────────────────────────
    // opcode = 0b0110011 = 0x33 for all R-type arithmetic
    { "ADD",  Format::R, 0x33, 0x0, 0x00 },
    { "SUB",  Format::R, 0x33, 0x0, 0x20 },
    { "SLL",  Format::R, 0x33, 0x1, 0x00 },   
    { "SLT",  Format::R, 0x33, 0x2, 0x00 },   
    { "SLTU", Format::R, 0x33, 0x3, 0x00 },   
    { "XOR",  Format::R, 0x33, 0x4, 0x00 },   
    { "SRL",  Format::R, 0x33, 0x5, 0x00 },   
    { "SRA",  Format::R, 0x33, 0x5, 0x20 },   
    { "OR",   Format::R, 0x33, 0x6, 0x00 },   
    { "AND",  Format::R, 0x33, 0x7, 0x00 },   
 
    // ── I-type: immediate arithmetic ──────────────────────────────────────
    // opcode = 0b0010011 = 0x13
    { "ADDI",  Format::I, 0x13, 0x0, 0x00 },   
    { "SLTI",  Format::I, 0x13, 0x2, 0x00 },   
    { "SLTIU", Format::I, 0x13, 0x3, 0x00 },   
    { "XORI",  Format::I, 0x13, 0x4, 0x00 },   
    { "ORI",   Format::I, 0x13, 0x6, 0x00 },   
    { "ANDI",  Format::I, 0x13, 0x7, 0x00 },   
    { "SLLI",  Format::I, 0x13, 0x1, 0x00 },
    { "SRLI",  Format::I, 0x13, 0x5, 0x00 },   
    { "SRAI",  Format::I, 0x13, 0x5, 0x20 },
 
    // ── I-type: loads ─────────────────────────────────────────────────────
    // opcode = 0b0000011 = 0x03
    { "LB",  Format::I, 0x03, 0x0, 0x00 },   
    { "LH",  Format::I, 0x03, 0x1, 0x00 },   
    { "LW",  Format::I, 0x03, 0x2, 0x00 },   
    { "LBU", Format::I, 0x03, 0x4, 0x00 },   
    { "LHU", Format::I, 0x03, 0x5, 0x00 },   
 
    // ── I-type: JALR ──────────────────────────────────────────────────────
    // opcode = 0b1100111 = 0x67
    { "JALR", Format::I, 0x67, 0x0, 0x00 },   
 
    // ── S-type: stores ────────────────────────────────────────────────────
    // opcode = 0b0100011 = 0x23
    { "SB", Format::S, 0x23, 0x0, 0x00 },   
    { "SH", Format::S, 0x23, 0x1, 0x00 },   
    { "SW", Format::S, 0x23, 0x2, 0x00 },   
 
    // ── B-type: branches ──────────────────────────────────────────────────
    // opcode = 0b1100011 = 0x63
    { "BEQ",  Format::B, 0x63, 0x0, 0x00 },   
    { "BNE",  Format::B, 0x63, 0x1, 0x00 },   
    { "BLT",  Format::B, 0x63, 0x4, 0x00 },   
    { "BGE",  Format::B, 0x63, 0x5, 0x00 },   
    { "BLTU", Format::B, 0x63, 0x6, 0x00 },   
    { "BGEU", Format::B, 0x63, 0x7, 0x00 },   
 
    // ── U-type ────────────────────────────────────────────────────────────
    { "LUI",   Format::U, 0x37, 0x0, 0x00 },   
    { "AUIPC", Format::U, 0x17, 0x0, 0x00 },   
 
    // ── J-type ────────────────────────────────────────────────────────────
    { "JAL", Format::J, 0x6F, 0x0, 0x00 },   
 
    // ── System ────────────────────────────────────────────────────────────
    { "ECALL",  Format::SYSTEM, 0x73, 0x0, 0x00 },
    { "EBREAK", Format::SYSTEM, 0x73, 0x0, 0x00 },
    { "FENCE",  Format::SYSTEM, 0x0F, 0x0, 0x00 },
};

static const size_t INSTRUCTION_TABLE_SIZE = sizeof(INSTRUCTION_TABLE) / sizeof(INSTRUCTION_TABLE[0]);