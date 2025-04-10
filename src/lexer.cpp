#include "lexer.hpp"
#include <cctype>
#include <iostream>

void Lexer::advance() {
    position++;
    currentChar = infix.at(position);
}

bool Lexer::isVariable(char c) { return c >= 'a' && c <= 'z'; }

void Lexer::reportError(std::string error, int offset) {
    if (offset < 0) {
        offset = position;
    }
    std::cout << infix << "\n";
    std::cout << std::string(offset, ' ');
    std::cout << COLOR_RED << "^ " << error << COLOR_RESET << std::endl;
}

Lexer::Lexer(const std::string& input) : infix(input) {
    position = -1;
    rank = 0;
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    advance(); // not checking for exception here because empty input is
               // ignored by the input loop
    bool reachedEnd = false;
    while (!reachedEnd) {
        if (std::isspace(currentChar)) {
            goto next;
        }

        if (currentChar == '|') {
            tokens.push_back(OrToken());
        } else if (currentChar == '!') {
            tokens.push_back(NegationToken());
        } else if (currentChar == '&') {
            tokens.push_back(AndToken());
        } else if (currentChar == '^') {
            tokens.push_back(XorToken());
        } else if (currentChar == '>') {
            tokens.push_back(ImplicationToken());
        } else if (currentChar == '=') {
            tokens.push_back(BiconditionalToken());
        } else if (currentChar == '(') {
            tokens.push_back(LPARENToken());
            bracketPositions.push(position);
        } else if (currentChar == ')') {
            if (bracketPositions.empty()) {
                // no open bracket is set for this to close
                reportError("missing opening parentheses");
                return std::vector<Token>{};
            }
            tokens.push_back(RPARENToken());
            bracketPositions.pop();
        } else if (isVariable(currentChar)) {
            auto tempToken = VariableToken(std::string(1, currentChar));
            tokens.push_back(tempToken);
        } else {
            reportError("invalid character");
            return std::vector<Token>{};
        }

        {
            // * new scope to allow control transfer from goto
            auto lastToken = tokens.back();
            auto isVariable = lastToken.isVariable();
            auto isParen = lastToken.isParen();
            auto isUnary = lastToken.isUnaryOperator();
            if (isUnary) {
                // ignore unary operators in rank calculation
                goto next;
            } else if (isVariable) {
                rank++;
            } else if (!isParen) {
                rank--;
            }

            if (rank < 0) {
                reportError("missing operand");
                return std::vector<Token>{};
            } else if (rank > 1) {
                reportError("missing operator");
                return std::vector<Token>{};
            }
        }

    next: {
        try {
            advance();
        } catch (std::out_of_range) {
            reachedEnd = true;
        }
    }
    }

    // rank checking is different here because the final value of an
    // expression must be equal to one. the range of rank is [0, 1] inside
    // loop.
    if (rank < 1) {
        reportError("missing operand");
        return std::vector<Token>{};
    } else if (rank > 1) {
        reportError("missing operator");
        return std::vector<Token>{};
    }

    if (!bracketPositions.empty()) {
        reportError("missing closing parentheses", bracketPositions.top());
        return std::vector<Token>{};
    }
    return tokens;
}
