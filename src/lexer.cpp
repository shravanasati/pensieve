#include "lexer.h"
#include <cctype>
#include <iostream>

void Lexer::advance() {
    position++;
    currentChar = infix.at(position);
}

bool Lexer::isDigit(char c) { return c >= '0' && c <= '9'; }

bool Lexer::isDecimal(char c) { return c == '.'; }

makeTokenResult Lexer::makeNumberToken() {
    std::string number;
    int decimalCount = 0;
    while (true) {
        if (isDigit(currentChar) || isDecimal(currentChar)) {
            number += currentChar;
            if (isDecimal(currentChar)) {
                decimalCount++;
            }
            if (decimalCount > 1) {
                reportError("multiple decimals in a number");
                return makeTokenResult{NumberToken("0"), false};
            }
            try {
                advance();
            } catch (std::out_of_range) {
                return makeTokenResult{NumberToken(number), true};
            }
        } else {
            // since current char is not a digit or a decimal
            // it maybe a operator which will be advanced upon
            // inside the loop, so decrease the position by 1
            position--;
            break;
        }
    }
    // convert .52 => 0.52
    if (number[0] == '.') {
        number = "0" + number;
    }
    // convert 52. => 52.0
    if (number.at(number.size() - 1) == '.') {
        number += "0";
    }
    // convert 52 => 52.0
    if (decimalCount == 0) {
        number += ".0";
    }
    return makeTokenResult{NumberToken(number), true};
}

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

        if (currentChar == '+') {
            auto n = tokens.size();
            if (n == 0) {
                tokens.push_back(UnaryPlusToken());
            } else {
                auto lastTokenType = tokens[n - 1].getTokenType();
                if (lastTokenType == TokenType::NUMBER ||
                    lastTokenType == TokenType::RPAREN) {
                    tokens.push_back(PlusToken());
                } else {
                    tokens.push_back(UnaryPlusToken());
                }
            }
        } else if (currentChar == '-') {
            auto n = tokens.size();
            if (n == 0) {
                tokens.push_back(UnaryMinusToken());
            } else {
                auto lastTokenType = tokens[n - 1].getTokenType();
                if (lastTokenType == TokenType::NUMBER ||
                    lastTokenType == TokenType::RPAREN) {
                    tokens.push_back(MinusToken());
                } else {
                    tokens.push_back(UnaryMinusToken());
                }
            }
        } else if (currentChar == '*') {
            tokens.push_back(MultiplyToken());
        } else if (currentChar == '/') {
            if (position == (infix.length() - 1)) {
                // no need to check for floor division if the end of input
                // is reached
                tokens.push_back(DivideToken());
            } else {
                // try advancing to check for floor division operator
                advance();
                if (currentChar == '/') {
                    tokens.push_back(FloorDivideToken());
                } else {
                    tokens.push_back(DivideToken());
                    // go back one character since we advanced once more to
                    // check next character
                    position--;
                }
            }
        } else if (currentChar == '^') {
            tokens.push_back(ExponentToken());
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
        } else if (isDigit(currentChar) || isDecimal(currentChar)) {
            auto numberTokenResult = makeNumberToken();
            if (!numberTokenResult.success) {
                return std::vector<Token>{};
            }
            tokens.push_back(numberTokenResult.token);
        } else {
            reportError("invalid character");
            return std::vector<Token>{};
        }

        {
            // * new scope to allow control transfer from goto
            auto lastToken = tokens.back();
            auto isNumber = lastToken.isNumber();
            auto isParen = lastToken.isParen();
            auto isUnary = lastToken.isUnaryOperator();
            if (isUnary) {
                // ignore unary operators in rank calculation
                goto next;
            } else if (isNumber) {
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
