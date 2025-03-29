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

bool Token::isVariable() { return tokenType == TokenType::VARIABLE; }

bool Token::isUnaryOperator() { return tokenType == TokenType::NEGATION_OP; }

bool Token::isParen() {
    return tokenType == TokenType::LPAREN || tokenType == TokenType::RPAREN;
}

Token VariableToken(std::string name) {
    return Token(name, TokenType::VARIABLE, 0, Associativity::LEFT);
}

Token NegationToken() {
    return Token("!", TokenType::NEGATION_OP, 6, Associativity::RIGHT);
}

Token OrToken() { return Token("|", TokenType::OR_OP, 3, Associativity::LEFT); }


Token AndToken() {
    return Token("&", TokenType::AND_OP, 5, Associativity::LEFT);
}

Token XorToken() {
    return Token("^", TokenType::XOR_OP, 4, Associativity::LEFT);
}

Token LPARENToken() {
    return Token("(", TokenType::LPAREN, 0, Associativity::LEFT);
}

Token RPARENToken() {
    return Token(")", TokenType::RPAREN, 0, Associativity::LEFT);
}

Token ImplicationToken() {
    return Token(">", TokenType::IMLPICATION_OP, 2, Associativity::RIGHT);
}

Token BiconditionalToken() {
    return Token("=", TokenType::BICONDITIONAL_OP, 1, Associativity::LEFT);
}
