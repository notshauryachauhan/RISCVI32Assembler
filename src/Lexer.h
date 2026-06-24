#pragma once
#include <string>
#include <vector>

class Lexer {
public:
    
    std::vector<std::vector<std::string>> tokenise(const std::string& filePath);

private:
    std::string stripComment(const std::string& line);

    std::string trim(const std::string& line);

    std::vector<std::string> split(const std::string& line);
};