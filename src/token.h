#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <vector>

enum class Associativity { LEFT, RIGHT };

enum class TokenType {
    NUMBER,
    UNARY_PLUS_OP,
    UNARY_MINUS_OP,
    PLUS_OP,
    MINUS_OP,
    MULTIPLY_OP,
    DIVIDE_OP,
    FLOOR_DIVIDE_OP,
    EXPONENT_OP,
    LPAREN,
    RPAREN
};

class Token {
protected:
    std::string value;
    TokenType tokenType;
    int precedence;
    Associativity associativity;

public:
    Token(const std::string& value, TokenType tokenType, int precedence,
          Associativity associativity);

    std::string getValue() const;
    TokenType getTokenType() const;
    int getPrecedence() const;
    Associativity getAssociativity() const;

    bool operator==(const Token& other) const;
    bool isNumber();
    bool isUnaryOperator();
    bool isParen();
};

// Factory functions for creating tokens
Token NumberToken(std::string value);
Token UnaryPlusToken();
Token UnaryMinusToken();
Token PlusToken();
Token MinusToken();
Token MultiplyToken();
Token DivideToken();
Token FloorDivideToken();
Token ExponentToken();
Token LPARENToken();
Token RPARENToken();

#endif // TOKEN_H
