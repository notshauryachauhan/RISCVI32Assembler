#include "../include/SecondPass.h"
#include "../include/FirstPass.h"
#include "../include/Encoder.h"
 
std::vector<uint32_t> SecondPass::run(const std::vector<std::vector<std::string>>& tokens, const SymbolTable& symbolTable) {
    std::vector<uint32_t> machineCode;
    Encoder encoder;
    int currentAddress = 0;
 
    for (const auto& line : tokens) {
        if (line.empty()) continue;

        if(FirstPass::isLabel(line[0])) {
            if(line.size() == 1) {
                continue;
            }
            machineCode.push_back(encoder.encode(std::vector<std::string>(line.begin() + 1, line.end()), currentAddress, symbolTable));
            currentAddress += 4;
            continue;
        }

        machineCode.push_back(encoder.encode(line, currentAddress, symbolTable));

        currentAddress += 4;
    }

    machineCode.push_back(0);

    return machineCode;

}