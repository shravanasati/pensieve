#include "interpreter.h"
#include <cmath>
#include <sstream>
#include <stdexcept>

void Interpreter::convertToPostfix() {
    for (auto& token : infixTokens) {
        auto tokenType = token.getTokenType();
        auto tokenPrecedence = token.getPrecedence();
        auto tokenAssoc = token.getAssociativity();

        if (tokenType == TokenType::NUMBER) {
            postfixTokens.push_back(token);
        } else {
            if (operatorStack.empty() || tokenType == TokenType::LPAREN) {
                operatorStack.push(token);
                continue;
            }
            if (tokenType == TokenType::RPAREN) {
                while (operatorStack.top().getTokenType() !=
                       TokenType::LPAREN) {
                    postfixTokens.push_back(operatorStack.top());
                    operatorStack.pop();
                    // we need not worry about stack being empty because
                    // lparen is 100% present due to checking by lexer
                }
                operatorStack.pop(); // remove LPAREN from operator stack
                continue;
            }
            auto topOperator = operatorStack.top();
            if (tokenPrecedence > topOperator.getPrecedence()) {
                operatorStack.push(token);
            } else if (tokenPrecedence < topOperator.getPrecedence()) {
                while (operatorStack.top().getPrecedence() >= tokenPrecedence) {
                    postfixTokens.push_back(operatorStack.top());
                    operatorStack.pop();
                    if (operatorStack.empty()) {
                        break;
                    }
                }
                operatorStack.push(token);
            } else {
                if (tokenAssoc == Associativity::LEFT) {
                    postfixTokens.push_back(operatorStack.top());
                    operatorStack.pop();
                    operatorStack.push(token);
                } else {
                    operatorStack.push(token);
                }
            }
        }
    }

    while (!operatorStack.empty()) {
        postfixTokens.push_back(operatorStack.top());
        operatorStack.pop();
    }
}

long double Interpreter::resolveOperator(Token token,
                                         std::stack<long double>& operands) {

    if (token.isUnaryOperator()) {
        auto num = operands.top();
        operands.pop();
        auto sign = (token.getTokenType() == TokenType::UNARY_PLUS_OP) ? 1 : -1;
        return sign * num;
    }

    auto tokenType = token.getTokenType();
    auto num2 = operands.top();
    operands.pop();
    auto num1 = operands.top();
    operands.pop();
    switch (tokenType) {
    case TokenType::PLUS_OP:
        return (num1 + num2);
        break;
    case TokenType::MINUS_OP:
        return (num1 - num2);
        break;

    case TokenType::MULTIPLY_OP:
        return (num1 * num2);
        break;
    case TokenType::DIVIDE_OP:
        return (num1 / num2);
        break;
    case TokenType::FLOOR_DIVIDE_OP:
        return (int(num1 / num2));
        break;

    case TokenType::EXPONENT_OP:
        return (pow(num1, num2));
        break;

    default:
        throw std::logic_error("unknown token type to resolve");
        break;
    }
}

double Interpreter::evalPostfix() {
    std::stack<long double> operands;
    for (auto token : postfixTokens) {
        if (token.isNumber()) {
            operands.push(std::stold(token.getValue()));
        } else {
            operands.push(resolveOperator(token, operands));
        }
    }
    return operands.top();
}

Interpreter::Interpreter(std::vector<Token> tokens) : infixTokens(tokens) {
    convertToPostfix();
};

std::string Interpreter::getPostfix() {
    std::stringstream ss;
    for (auto& tok : postfixTokens) {
        ss << tok.getValue() << " ";
    }
    return ss.str();
}

std::string Interpreter::getInfix() {
    std::stack<std::string> expr;
    for (auto& token : postfixTokens) {
        if (token.isNumber()) {
            expr.push(token.getValue());
        } else if (token.isUnaryOperator()) {
            auto e1 = expr.top();
            expr.pop();
            std::string sign =
                (token.getTokenType() == TokenType::UNARY_PLUS_OP) ? "+" : "-";
            auto e = sign + "(" + e1 + ")";
            expr.push(e);
        } else {
            auto e2 = expr.top();
            expr.pop();
            auto e1 = expr.top();
            expr.pop();
            auto e = "(" + e1 + " " + token.getValue() + " " + e2 + ")";
            expr.push(e);
        }
    }
    return expr.top();
}

double Interpreter::evaluate() { return evalPostfix(); }
