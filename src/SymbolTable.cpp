#include "SymbolTable.h"
#include <iostream>
#include <stdexcept>

class SymbolTable {
    public:

        void addLabel(const std::string& name, int address) {
            if (table.find(name) != table.end()) {
                std::cerr << "Warning: Duplicate label '" << name << "' defined." << std::endl;
            }
            table[name] = address;
        }

        int getAddress(const std::string& name) const {
            auto it = table.find(name);
            if (it == table.end()) {
                throw std::runtime_error("Label '" + name + "' not found.");
            }
            return it->second;
        }

        bool contains(const std::string& name) const {
            return table.find(name) != table.end();
        }

        void print() const {
            for (const auto& pair : table) {
                std::cout << pair.first << ": " << pair.second << std::endl;
            }
        }

    private:
        std::map<std::string, int> table;
};