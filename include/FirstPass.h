#pragma once
#include <vector>
#include <string>
#include "SymbolTable.h"
 

namespace FirstPass {
    void run(const std::vector<std::vector<std::string>>& tokens, SymbolTable& symbolTable);
    bool isLabel(const std::string& token);
}
 