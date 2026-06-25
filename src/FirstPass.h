#pragma once
#include <vector>
#include <string>
#include "SymbolTable.h"
 
// ── FirstPass ─────────────────────────────────────────────────────────────────
// Responsibility: scan the token list and populate the symbol table.
//
// Rules:
//   - A token ending in ':' is a label. Record it at the current address.
//   - A label on its own line does NOT increment the address.
//   - A label on the same line as an instruction: record label, then increment.
//   - Every instruction line increments the address by 4 (all RV32I = 4 bytes).
//   - Skip blank token lists (empty lines were already filtered by Lexer).
//
// After this pass, symbolTable contains every label and its byte address.
// No encoding happens here.
 
namespace FirstPass {
    // Run the first pass.
    // Modifies symbolTable in place.
    void run(const std::vector<std::vector<std::string>>& tokens, SymbolTable& symbolTable);
 
    // Returns true if a token is a label (ends with ':').
    bool isLabel(const std::string& token);
}
 