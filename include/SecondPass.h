#pragma once
#include <vector>
#include <string>
#include <cstdint>
#include "SymbolTable.h"

 
namespace SecondPass {
    
    std::vector<uint32_t> run(const std::vector<std::vector<std::string>>& tokens, const SymbolTable& symbolTable);
}
 