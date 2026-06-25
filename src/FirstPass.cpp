#include<vector>
#include<string>
#include "SymbolTable.h"
#include "FirstPass.h"

namespace FirstPass{
    void run(const std::vector<std::vector<std::string>>& tokens, SymbolTable& symbolTable){
        int currentAddress = 0;
        for (const auto& line : tokens) {
            if (line.empty()) {
                continue; // Skip empty lines
            }

            bool hasInstruction = false;
            for (const auto& token : line) {
                if (isLabel(token)) {
                    std::string labelName = token.substr(0, token.size() - 1);
                    symbolTable.addLabel(labelName, currentAddress);
                } else {
                    hasInstruction = true;
                }
            }

            if (hasInstruction) {
                currentAddress += 4;
            }
        }
    }

    bool isLabel(const std::string& token){
        return !token.empty() && token.back() == ':';
    }
}