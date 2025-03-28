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
	std::vector<std::string> variableNames;

	std::vector<Token> getVariableTokens();
    void convertToPostfix();
    long double resolveOperator(Token token, std::stack<long double>& operands);
    double evalPostfix();

public:
    Interpreter(std::vector<Token> tokens);
    std::string getPostfix();
    std::string getInfix();
    std::string getVariables();
    double evaluate();
};

#endif // INTERPRETER_H
