#pragma once
#include <string>
#include <map>
 
class SymbolTable {
public:
    void addLabel(const std::string& name, int address);
 
    int getAddress(const std::string& name) const;
 
    bool contains(const std::string& name) const;
 
    void print() const;
 
private:
    std::map<std::string, int> table;
};
 