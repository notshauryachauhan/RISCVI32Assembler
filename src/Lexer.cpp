#include "../include/Lexer.h"
#include <vector>
#include <string>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <algorithm>


std::vector<std::vector<std::string>> Lexer::tokenise(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filePath);
    }

            std::vector<std::vector<std::string>> tokens;

    std::string line;
    while (std::getline(file, line)) {
        line = stripComment(line);
        line = trim(line);
        if (!line.empty()) {
            tokens.push_back(split(line));
        }
    }
    return tokens;
}

std::string Lexer::stripComment(const std::string& line) {
    size_t commentPos = line.find('#');
    if (commentPos != std::string::npos){
        return line.substr(0, commentPos);
    }
    return line;
}

std::string Lexer::trim(const std::string& line){
    size_t start = line.find_first_not_of(" ");
    if (start == std::string::npos) {
        return "";
    }
    size_t end = line.find_last_not_of(" ");
    return line.substr(start, end - start + 1);
}

std::vector<std::string> Lexer::split(const std::string& line){
    std::vector<std::string> tokens;
    std::string token;
    std::string modifiedLine = line;
    std::replace(modifiedLine.begin(), modifiedLine.end(), ',', ' ');
    std::istringstream stream(modifiedLine);

    while (stream >> token) {
        tokens.push_back(token);
        }

    return tokens;
}