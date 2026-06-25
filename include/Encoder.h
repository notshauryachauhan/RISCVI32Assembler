#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include "SymbolTable.h"
#include "InstructionTable.h"
 
 
class Encoder {
public:
    uint32_t encode(const std::vector<std::string>& tokens,
                    int currentAddr,
                    const SymbolTable& symbolTable);
 
private:
 
    uint32_t encodeR(uint32_t funct7, uint32_t rs2, uint32_t rs1,
                     uint32_t funct3, uint32_t rd, uint32_t opcode);
 
    uint32_t encodeI(uint32_t imm, uint32_t rs1,
                     uint32_t funct3, uint32_t rd, uint32_t opcode);
 
    uint32_t encodeS(uint32_t imm, uint32_t rs2, uint32_t rs1,
                     uint32_t funct3, uint32_t opcode);
 
    uint32_t encodeB(int imm, uint32_t rs2, uint32_t rs1,
                     uint32_t funct3, uint32_t opcode);
 
    uint32_t encodeU(uint32_t imm, uint32_t rd, uint32_t opcode);
 
    uint32_t encodeJ(int imm, uint32_t rd, uint32_t opcode);
 
    uint32_t regNum(const std::string& name);
 
    void parseMemOperand(const std::string& token, int& offset, std::string& baseReg);
 
    const InstructionInfo& lookup(const std::string& mnemonic);
};