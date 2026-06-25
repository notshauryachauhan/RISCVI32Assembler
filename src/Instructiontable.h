#pragma once
#include <cstdint>
#include <string>
 
// ── Instruction format types ──────────────────────────────────────────────────
// Every RV32I instruction belongs to exactly one of these 6 formats.
// The format determines which bit fields exist and how the immediate is encoded.
enum class Format {
    R,   // register-register  (ADD, SUB, AND, OR, XOR, SLL, SRL, SRA, SLT, SLTU)
    I,   // immediate + loads  (ADDI, LW, LH, LB, LHU, LBU, JALR, SLLI, SRLI, SRAI, ...)
    S,   // stores             (SW, SH, SB)
    B,   // branches           (BEQ, BNE, BLT, BGE, BLTU, BGEU)
    U,   // upper immediate    (LUI, AUIPC)
    J,   // jump               (JAL)
    SYSTEM  // FENCE, ECALL, EBREAK — handled as hardcoded values
};
 
// ── One entry per instruction ─────────────────────────────────────────────────
struct InstructionInfo {
    std::string mnemonic;   // e.g. "ADD", "ADDI", "BEQ"
    Format      format;     // which encoding format to use
    uint32_t    opcode;     // 7-bit opcode  (bits [6:0])
    uint32_t    funct3;     // 3-bit funct3  (bits [14:12]) — 0 if unused
    uint32_t    funct7;     // 7-bit funct7  (bits [31:25]) — 0 if unused
};
 
// ── The full RV32I instruction table ─────────────────────────────────────────
// TODO: fill in the correct opcode/funct3/funct7 values from the RISC-V spec
//       as you complete Phase 1 (your paper opcode table translates directly here).
//
// Reference: RISC-V Unprivileged Spec, Chapter 2, Table 24.2
//
// FORMAT:  { "MNEMONIC", Format::X, opcode, funct3, funct7 }
 
static const InstructionInfo INSTRUCTION_TABLE[] = {
 
    // ── R-type ────────────────────────────────────────────────────────────
    // opcode = 0b0110011 = 0x33 for all R-type arithmetic
    { "ADD",  Format::R, 0x33, 0x0, 0x00 },  // TODO: verify from spec
    { "SUB",  Format::R, 0x33, 0x0, 0x20 },  // TODO: verify from spec
    { "SLL",  Format::R, 0x33, 0x1, 0x00 },  // TODO: verify
    { "SLT",  Format::R, 0x33, 0x2, 0x00 },  // TODO: verify
    { "SLTU", Format::R, 0x33, 0x3, 0x00 },  // TODO: verify
    { "XOR",  Format::R, 0x33, 0x4, 0x00 },  // TODO: verify
    { "SRL",  Format::R, 0x33, 0x5, 0x00 },  // TODO: verify
    { "SRA",  Format::R, 0x33, 0x5, 0x20 },  // TODO: verify
    { "OR",   Format::R, 0x33, 0x6, 0x00 },  // TODO: verify
    { "AND",  Format::R, 0x33, 0x7, 0x00 },  // TODO: verify
 
    // ── I-type: immediate arithmetic ──────────────────────────────────────
    // opcode = 0b0010011 = 0x13
    { "ADDI",  Format::I, 0x13, 0x0, 0x00 },  // TODO: verify
    { "SLTI",  Format::I, 0x13, 0x2, 0x00 },  // TODO: verify
    { "SLTIU", Format::I, 0x13, 0x3, 0x00 },  // TODO: verify
    { "XORI",  Format::I, 0x13, 0x4, 0x00 },  // TODO: verify
    { "ORI",   Format::I, 0x13, 0x6, 0x00 },  // TODO: verify
    { "ANDI",  Format::I, 0x13, 0x7, 0x00 },  // TODO: verify
    { "SLLI",  Format::I, 0x13, 0x1, 0x00 },  // TODO: verify (shamt in imm[4:0])
    { "SRLI",  Format::I, 0x13, 0x5, 0x00 },  // TODO: verify
    { "SRAI",  Format::I, 0x13, 0x5, 0x20 },  // TODO: verify (funct7 distinguishes from SRLI)
 
    // ── I-type: loads ─────────────────────────────────────────────────────
    // opcode = 0b0000011 = 0x03
    { "LB",  Format::I, 0x03, 0x0, 0x00 },  // TODO: verify
    { "LH",  Format::I, 0x03, 0x1, 0x00 },  // TODO: verify
    { "LW",  Format::I, 0x03, 0x2, 0x00 },  // TODO: verify
    { "LBU", Format::I, 0x03, 0x4, 0x00 },  // TODO: verify
    { "LHU", Format::I, 0x03, 0x5, 0x00 },  // TODO: verify
 
    // ── I-type: JALR ──────────────────────────────────────────────────────
    // opcode = 0b1100111 = 0x67
    { "JALR", Format::I, 0x67, 0x0, 0x00 },  // TODO: verify
 
    // ── S-type: stores ────────────────────────────────────────────────────
    // opcode = 0b0100011 = 0x23
    { "SB", Format::S, 0x23, 0x0, 0x00 },  // TODO: verify
    { "SH", Format::S, 0x23, 0x1, 0x00 },  // TODO: verify
    { "SW", Format::S, 0x23, 0x2, 0x00 },  // TODO: verify
 
    // ── B-type: branches ──────────────────────────────────────────────────
    // opcode = 0b1100011 = 0x63
    { "BEQ",  Format::B, 0x63, 0x0, 0x00 },  // TODO: verify
    { "BNE",  Format::B, 0x63, 0x1, 0x00 },  // TODO: verify
    { "BLT",  Format::B, 0x63, 0x4, 0x00 },  // TODO: verify
    { "BGE",  Format::B, 0x63, 0x5, 0x00 },  // TODO: verify
    { "BLTU", Format::B, 0x63, 0x6, 0x00 },  // TODO: verify
    { "BGEU", Format::B, 0x63, 0x7, 0x00 },  // TODO: verify
 
    // ── U-type ────────────────────────────────────────────────────────────
    { "LUI",   Format::U, 0x37, 0x0, 0x00 },  // TODO: verify
    { "AUIPC", Format::U, 0x17, 0x0, 0x00 },  // TODO: verify
 
    // ── J-type ────────────────────────────────────────────────────────────
    { "JAL", Format::J, 0x6F, 0x0, 0x00 },  // TODO: verify
 
    // ── System ────────────────────────────────────────────────────────────
    { "ECALL",  Format::SYSTEM, 0x73, 0x0, 0x00 },
    { "EBREAK", Format::SYSTEM, 0x73, 0x0, 0x00 },
    { "FENCE",  Format::SYSTEM, 0x0F, 0x0, 0x00 },
};

static const size_t INSTRUCTION_TABLE_SIZE = sizeof(INSTRUCTION_TABLE) / sizeof(INSTRUCTION_TABLE[0]);