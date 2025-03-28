#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "token.h"
#include <stack>
#include <string>
#include <vector>

class Interpreter {
private:
    std::vector<Token> infixTokens;
    std::vector<Token> postfixTokens;
    std::stack<Token> operatorStack;
	int variableCount;

	int countVariables();
    void convertToPostfix();
    long double resolveOperator(Token token, std::stack<long double>& operands);
    double evalPostfix();

public:
    Interpreter(std::vector<Token> tokens);
    std::string getPostfix();
    std::string getInfix();
    double evaluate();
};

#endif // INTERPRETER_H
