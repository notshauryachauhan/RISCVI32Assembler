#include "../include/Encoder.h"
#include <stdexcept>
#include <unordered_map>

uint32_t Encoder::encode(const std::vector<std::string>& tokens, int currentAddr, const SymbolTable& symbolTable) {
    const InstructionInfo& info = lookup(tokens[0]);
    switch(info.format) {
        case Format::R: {
            uint32_t rd = regNum(tokens[1]);
            uint32_t rs1 = regNum(tokens[2]);
            uint32_t rs2 = regNum(tokens[3]);
            return encodeR(info.funct7, rs2, rs1, info.funct3, rd, info.opcode);
        }
        case Format::I: {
            uint32_t rd = regNum(tokens[1]);
            uint32_t rs1;
            int imm;
            if (tokens[0] == "LW" || tokens[0] == "LH" || tokens[0] == "LB" || tokens[0] == "LHU" || tokens[0] == "LBU" || tokens[0] == "JALR") {
                std::string baseReg;
                parseMemOperand(tokens[2], imm, baseReg);
                rs1 = regNum(baseReg);
            } else {
                rs1 = regNum(tokens[2]);
                imm = std::stoi(tokens[3]);
            }
            return encodeI(imm, rs1, info.funct3, rd, info.opcode);
        }
        case Format::S: {
            uint32_t rs2 = regNum(tokens[1]);
            int imm;
            std::string baseReg;
            parseMemOperand(tokens[2], imm, baseReg);
            uint32_t rs1 = regNum(baseReg);
            return encodeS(imm, rs2, rs1, info.funct3, info.opcode);
        }
        case Format::B: {
            uint32_t rs1 = regNum(tokens[1]);
            uint32_t rs2 = regNum(tokens[2]);
            int imm = symbolTable.getAddress(tokens[3]) - currentAddr;
            return encodeB(imm, rs2, rs1, info.funct3, info.opcode);
        }
        case Format::U: {
            uint32_t rd = regNum(tokens[1]);
            int imm = std::stoi(tokens[2], nullptr, 0);
            return encodeU(imm, rd, info.opcode);
        }
        case Format::J: {
            uint32_t rd = regNum(tokens[1]);
            int imm = symbolTable.getAddress(tokens[2]) - currentAddr;
            return encodeJ(imm, rd, info.opcode);
        }
        case Format::SYSTEM:
            if (tokens[0] == "ECALL") {
                return 0x00000073;
            } else if (tokens[0] == "EBREAK") {
                return 0x00100073;
            } else if (tokens[0] == "FENCE") {
                return 0x0000000F;
            } else {
                throw std::runtime_error("Unknown SYSTEM instruction: " + tokens[0]);
            }
        default:
            throw std::runtime_error("Unsupported instruction format for: " + tokens[0]);   
    }
}


uint32_t Encoder::encodeR(uint32_t funct7, uint32_t rs2, uint32_t rs1, uint32_t funct3, uint32_t rd, uint32_t opcode) {
    return (funct7 << 25) | (rs2 << 20) | (rs1 << 15) | (funct3 << 12) | (rd << 7) | opcode;
}


uint32_t Encoder::encodeI(uint32_t imm, uint32_t rs1, uint32_t funct3, uint32_t rd, uint32_t opcode) {
        return ((imm & 0xFFF) << 20) | (rs1 << 15) | (funct3 << 12) | (rd << 7) | opcode;
}

uint32_t Encoder::encodeS(uint32_t imm, uint32_t rs2, uint32_t rs1, uint32_t funct3, uint32_t opcode) {
    uint32_t imm11_5 = (imm >> 5) & 0x7F;
    uint32_t imm4_0 = imm & 0x1F;
    return (imm11_5 << 25) | (rs2 << 20) | (rs1 << 15) | (funct3 << 12) | (imm4_0 << 7) | opcode;
}

uint32_t Encoder::encodeB(int imm, uint32_t rs2, uint32_t rs1, uint32_t funct3, uint32_t opcode) {
    uint32_t imm12 = (imm >> 12) & 0x1;
    uint32_t imm10_5 = (imm >> 5) & 0x3F;
    uint32_t imm4_1 = (imm >> 1) & 0xF;
    uint32_t imm11 = (imm >> 11) & 0x1;
        return (imm12 << 31) | (imm11 << 7) | (imm10_5 << 25) | (rs2 << 20) | (rs1 << 15) | (funct3 << 12) | (imm4_1 << 8) | opcode;
}

 uint32_t Encoder::encodeU(uint32_t imm, uint32_t rd, uint32_t opcode) {
    return (imm << 12) | (rd << 7) | opcode;
}

uint32_t Encoder::encodeJ(int imm, uint32_t rd, uint32_t opcode) {
    uint32_t imm20 = (imm >> 20) & 0x1;
    uint32_t imm10_1 = (imm >> 1) & 0x3FF;
    uint32_t imm11 = (imm >> 11) & 0x1;
    uint32_t imm19_12 = (imm >> 12) & 0xFF;
    return (imm20 << 31) | (imm19_12 << 12) | (imm11 << 20) | (imm10_1 << 21) | (rd << 7) | opcode;
}
uint32_t Encoder::regNum(const std::string& name){
    static const std::unordered_map<std::string, uint32_t> regMap = {
        {"x0", 0}, {"zero", 0},
        {"x1", 1}, {"ra", 1},
        {"x2", 2}, {"sp", 2},
        {"x3", 3}, {"gp", 3},
        {"x4", 4}, {"tp", 4},
        {"x5", 5}, {"t0", 5},
        {"x6", 6}, {"t1", 6},
        {"x7", 7}, {"t2", 7},
        {"x8", 8}, {"s0", 8}, {"fp", 8},
        {"x9", 9}, {"s1", 9},
        {"x10", 10}, {"a0", 10},
        {"x11", 11}, {"a1", 11},
        {"x12", 12}, {"a2", 12},
        {"x13", 13}, {"a3", 13},
        {"x14", 14}, {"a4", 14},
        {"x15", 15}, {"a5", 15},
        {"x16", 16}, {"a6", 16},
        {"x17", 17}, {"a7", 17},
        {"x18", 18}, {"s2", 18},
        {"x19", 19}, {"s3", 19},
        {"x20", 20}, {"s4", 20},
        {"x21", 21}, {"s5", 21},
        {"x22", 22}, {"s6", 22},
        {"x23", 23}, {"s7", 23},
        {"x24", 24}, {"s8", 24},
        {"x25", 25}, {"s9", 25},
        {"x26", 26}, {"s10", 26},
        {"x27", 27}, {"s11", 27},
        {"x28", 28}, {"t3", 28},
        {"x29", 29}, {"t4", 29},
        {"x30", 30}, {"t5", 30},
        {"x31",31 },{"t6" ,31}
    };
    auto it = regMap.find(name);
    if (it == regMap.end()) {
        throw std::runtime_error("Invalid register name: " + name);
    }
    return it->second;
}

void Encoder::parseMemOperand(const std::string& token, int& offset, std::string& baseReg) {
    size_t openParen = token.find('(');
    size_t closeParen = token.find(')');
    if (openParen == std::string::npos || closeParen == std::string::npos || openParen >= closeParen) {
        throw std::runtime_error("Invalid memory operand format: " + token);
    }
    offset = std::stoi(token.substr(0, openParen));
    baseReg = token.substr(openParen + 1, closeParen - openParen - 1);
}

const InstructionInfo& Encoder::lookup(const std::string& mnemonic) {
    for (const auto& instr : INSTRUCTION_TABLE) {
        if (instr.mnemonic == mnemonic) {
            return instr;
        }
    }
    throw std::runtime_error("Unknown instruction mnemonic: " + mnemonic);
}