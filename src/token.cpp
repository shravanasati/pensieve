#include "token.h"

Token::Token(const std::string& value, TokenType tokenType, int precedence,
             Associativity associativity)
    : value(value), tokenType(tokenType), precedence(precedence),
      associativity(associativity) {}

std::string Token::getValue() const { return value; }
TokenType Token::getTokenType() const { return tokenType; }
int Token::getPrecedence() const { return precedence; }
Associativity Token::getAssociativity() const { return associativity; }

bool Token::operator==(const Token& other) const {
    return tokenType == other.tokenType;
}

bool Token::isNumber() { return tokenType == TokenType::NUMBER; }

bool Token::isUnaryOperator() {
    return tokenType == TokenType::UNARY_PLUS_OP ||
           tokenType == TokenType::UNARY_MINUS_OP;
}

bool Token::isParen() {
    return tokenType == TokenType::LPAREN || tokenType == TokenType::RPAREN;
}

Token NumberToken(std::string value) {
    return Token(value, TokenType::NUMBER, 0, Associativity::LEFT);
}

Token UnaryPlusToken() {
    return Token("+", TokenType::UNARY_PLUS_OP, 3, Associativity::RIGHT);
}

Token UnaryMinusToken() {
    return Token("-", TokenType::UNARY_MINUS_OP, 3, Associativity::RIGHT);
}

Token PlusToken() {
    return Token("+", TokenType::PLUS_OP, 1, Associativity::LEFT);
}

Token MinusToken() {
    return Token("-", TokenType::MINUS_OP, 1, Associativity::LEFT);
}

Token MultiplyToken() {
    return Token("*", TokenType::MULTIPLY_OP, 2, Associativity::LEFT);
}

Token DivideToken() {
    return Token("/", TokenType::DIVIDE_OP, 2, Associativity::LEFT);
}

Token FloorDivideToken() {
    return Token("//", TokenType::FLOOR_DIVIDE_OP, 2, Associativity::LEFT);
}

Token ExponentToken() {
    return Token("^", TokenType::EXPONENT_OP, 3, Associativity::RIGHT);
}

Token LPARENToken() {
    return Token("(", TokenType::LPAREN, 0, Associativity::LEFT);
}

Token RPARENToken() {
    return Token(")", TokenType::RPAREN, 0, Associativity::LEFT);
}
