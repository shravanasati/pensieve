#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <vector>

enum class Associativity { LEFT, RIGHT };

enum class TokenType {
    VARIABLE,
    NEGATION_OP,
    OR_OP,
    AND_OP,
    XOR_OP,
    IMLPICATION_OP,
    BICONDITIONAL_OP,
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
    bool isVariable();
    bool isUnaryOperator();
    bool isParen();
};

// Factory functions for creating tokens
Token VariableToken(std::string name);
Token NegationToken();
Token OrToken();
Token AndToken();
Token XorToken();
Token ImplicationToken();
Token BiconditionalToken();
Token LPARENToken();
Token RPARENToken();

#endif // TOKEN_H
