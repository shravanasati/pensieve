#ifndef LEXER_H
#define LEXER_H

#include "constants.h"
#include "token.h"
#include <stack>
#include <string>
#include <vector>

class Lexer {
private:
    const std::string& infix;
    int position;
    char currentChar;
    int rank;
    std::stack<int> bracketPositions;

    void advance();
    bool isVariable(char c);
    void reportError(std::string error, int offset = -1);

public:
    Lexer(const std::string& input);
    std::vector<Token> tokenize();
};

#endif // LEXER_H
