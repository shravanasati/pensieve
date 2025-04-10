#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "tokens.hpp"
#include <stack>
#include <string>
#include <vector>

class Interpreter {
private:
    std::vector<Token> infixTokens;
    std::vector<Token> postfixTokens;
    std::stack<Token> operatorStack;
    std::vector<std::string> variableNames;
    std::vector<std::vector<bool>> resultMatrix;

    std::vector<Token> getVariableTokens();
    void generateInitialMatrix();
    void convertToPostfix();
    long double resolveOperator(Token token, std::stack<bool>& operands);
    bool evalPostfix(int rowIdx);
    void displayResultMatrix();

public:
    Interpreter(std::vector<Token> tokens);
    std::string getPostfix();
    std::string getInfix();
    std::string getVariables();
    void evaluate();
};

#endif // INTERPRETER_H
