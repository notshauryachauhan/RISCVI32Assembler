#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cstdint>

#include "src/Lexer.h"
#include "src/SymbolTable.h"
#include "src/FirstPass.h"
#include "src/SecondPass.h"

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

    std::cout << "Assembled " << machineCode.size() << " instructions.\n";
    std::cout << "Output written to: " << outputFile << "\n";
 

}