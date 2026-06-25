#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cstdint>
#include <bitset>

#include "Lexer.h"
#include "SymbolTable.h"
#include "FirstPass.h"
#include "SecondPass.h"

int main(int argc, char* argv[]){
    if(argc != 3){
        std::cerr << "Enter correct arguments. 1st argument is the input file, 2nd is the output file." << std::endl;
        return 1;
    }

    std::string inputFile  = argv[1];
    std::string outputFile = argv[2];

    Lexer lexer;
    std::vector<std::vector<std::string>> tokens = lexer.tokenise(inputFile);

    SymbolTable symbolTable;

    FirstPass::run(tokens, symbolTable);

    std::vector<uint32_t> machineCode = SecondPass::run(tokens, symbolTable);

    std::ofstream outFileStream(outputFile);

    if (!outFileStream.is_open()) {
        std::cerr << "Error: Could not open output file " << outputFile << " for writing.\n";
        return 1;
    }

    for (uint32_t instruction : machineCode) {
        outFileStream << std::bitset<32>(instruction) << "\n";
    }

    outFileStream.close();

    std::cout << "Assembled " << machineCode.size() << " instructions.\n";
    std::cout << "Output written to: " << outputFile << "\n";

}