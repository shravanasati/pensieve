#include "interpreter.hpp"
#include "constants.hpp"
#include "tabulate.hpp"
#include <algorithm>
#include <cmath>
#include <sstream>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>

std::vector<Token> Interpreter::getVariableTokens() {
    std::vector<Token> variableTokens;
    std::unordered_set<std::string> seenNames;

    for (auto& tok : infixTokens) {
        if (tok.isVariable() &&
            (seenNames.find(tok.getValue()) == seenNames.end())) {
            variableTokens.push_back(tok);
            seenNames.insert(tok.getValue());
        }
    }
    return std::vector<Token>(variableTokens.begin(), variableTokens.end());
}

void Interpreter::generateInitialMatrix() {
    int varCount = this->variableNames.size();
    if (varCount == 0)
        return; // No variables, no matrix

    int totalEntries = (int)pow(2, varCount);
    resultMatrix.clear(); // Clear any previous data

    // Create one column per variable
    for (int i = 0; i < varCount; i++) {
        std::vector<bool> column(totalEntries);
        int blockSize = (int)pow(2, varCount - i - 1);

        // Fill the column with alternating blocks of true and false
        for (int j = 0; j < totalEntries; j++) {
            int block = (j / blockSize) % 2;
            column[j] = (block == 0);
        }

        resultMatrix.push_back(column);
    }
}

void Interpreter::convertToPostfix() {
    for (auto& token : infixTokens) {
        auto tokenType = token.getTokenType();
        auto tokenPrecedence = token.getPrecedence();
        auto tokenAssoc = token.getAssociativity();

        if (tokenType == TokenType::VARIABLE) {
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
                                         std::stack<bool>& operands) {

    auto tokenType = token.getTokenType();

    // Handle unary operators (like negation)
    if (token.isUnaryOperator()) {
        if (operands.empty()) {
            throw std::runtime_error(
                "Invalid expression: not enough operands for unary operator");
        }
        auto value = operands.top();
        operands.pop();
        // For NOT operation, invert the boolean value
        return !value;
    }

    // Handle binary operators
    if (operands.size() < 2) {
        throw std::runtime_error(
            "Invalid expression: not enough operands for binary operator");
    }

    auto num2 = operands.top();
    operands.pop();
    auto num1 = operands.top();
    operands.pop();

    switch (tokenType) {
    case TokenType::OR_OP:
        return (num1 || num2);
    case TokenType::AND_OP:
        return (num1 && num2);
    case TokenType::XOR_OP:
        return (num1 != num2);
    case TokenType::IMLPICATION_OP:
        return !(num1 && !num2);
    case TokenType::BICONDITIONAL_OP:
        return num1 == num2;
    default:
        throw std::logic_error("Unknown token type to resolve");
    }
}

bool Interpreter::evalPostfix(int rowIdx) {
    std::stack<bool> operands;
    std::unordered_map<std::string, int> variablePositions;

    // First, build a map of variable names to their positions in variableNames
    for (int i = 0; i < variableNames.size(); i++) {
        variablePositions[variableNames[i]] = i;
    }

    // Now evaluate the postfix expression
    for (auto& token : postfixTokens) {
        if (token.isVariable()) {
            // Look up the variable position and get its value from the result
            // matrix
            auto varName = token.getValue();
            if (variablePositions.find(varName) == variablePositions.end()) {
                throw std::runtime_error("Unknown variable: " + varName);
            }
            int varPos = variablePositions[varName];
            if (varPos >= resultMatrix.size() ||
                rowIdx >= resultMatrix[varPos].size()) {
                throw std::runtime_error(
                    "Matrix index out of bounds for variable: " + varName);
            }
            operands.push(resultMatrix[varPos][rowIdx]);
        } else {
            // Apply operator
            bool result = resolveOperator(token, operands);
            operands.push(result);
        }
    }

    if (operands.empty()) {
        throw std::runtime_error(
            "Invalid expression: evaluation resulted in no value");
    }

    return operands.top();
}

std::vector<bool> Interpreter::displayResultMatrix() {
    if (resultMatrix.empty() || variableNames.empty()) {
        std::cout << "No variables to display in truth table." << std::endl;
        return std::vector<bool>{};
    }

    tabulate::Table truthTable;

    // Add header row with variable names
    auto headerRow = tabulate::RowStream{};
    for (const auto& name : variableNames) {
        headerRow << name;
    }
    auto expr = getInfix();
    headerRow << expr;
    truthTable.add_row(headerRow);

    // Format header row
    truthTable.row(0)
        .format()
        .font_style({tabulate::FontStyle::bold})
        .font_align(tabulate::FontAlign::center);

    // Get the number of rows in the truth table
    int numRows = resultMatrix[0].size();

    // Add data rows
    std::vector<bool> resultCol;
    for (int rowIdx = 0; rowIdx < numRows; rowIdx++) {
        auto dataRow = tabulate::RowStream{};
        for (int colIdx = 0; colIdx < resultMatrix.size(); colIdx++) {
            dataRow << (resultMatrix[colIdx][rowIdx] ? "true" : "false");
        }
        auto result = evalPostfix(rowIdx);
        resultCol.push_back(result);
        dataRow << (result ? "true" : "false");
        truthTable.add_row(dataRow);
    }

    // Apply cell-level formatting
    // Format column headers
    for (size_t i = 0; i < variableNames.size(); i++) {
        truthTable[0][i].format().font_color(tabulate::Color::blue);
    }
    // Format the expression header
    truthTable[0][variableNames.size()].format().font_color(
        tabulate::Color::magenta);

    // Format data cells
    for (int rowIdx = 0; rowIdx < numRows; rowIdx++) {
        // Format variable columns
        for (int colIdx = 0; colIdx < resultMatrix.size(); colIdx++) {
            bool cellValue = resultMatrix[colIdx][rowIdx];
            truthTable[rowIdx + 1][colIdx]
                .format()
                .font_align(tabulate::FontAlign::center)
                .font_color(cellValue ? tabulate::Color::green
                                      : tabulate::Color::red);
        }

        // Format result column
        bool resultValue = resultCol[rowIdx];
        truthTable[rowIdx + 1][resultMatrix.size()]
            .format()
            .font_align(tabulate::FontAlign::center)
            .font_color(resultValue ? tabulate::Color::green
                                    : tabulate::Color::red)
            .font_style({tabulate::FontStyle::bold});
    }

    // Print the table
    std::cout << truthTable << std::endl;

    // check for all true or all false
    bool allTrue = std::all_of(resultCol.begin(), resultCol.end(),
                               [](bool val) { return val; });
    bool allFalse = std::all_of(resultCol.begin(), resultCol.end(),
                                [](bool val) { return !val; });
    if (allTrue) {
        std::cout << COLOR_YELLOW << "`" << expr << "` is a tautology"
                  << COLOR_RESET << std::endl;
    } else if (allFalse) {
        std::cout << COLOR_YELLOW << "`" << expr << "` is a contradiction"
                  << COLOR_RESET << std::endl;
    }

    return resultCol;
}

Interpreter::Interpreter(std::vector<Token> tokens) : infixTokens(tokens) {
    convertToPostfix();
    auto variableTokens = getVariableTokens();
    std::vector<std::string> varNames(variableTokens.size());
    std::transform(variableTokens.begin(), variableTokens.end(),
                   varNames.begin(), [](Token t) { return t.getValue(); });
    variableNames = varNames;
    generateInitialMatrix();
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
        if (token.isVariable()) {
            expr.push(token.getValue());
        } else if (token.isUnaryOperator()) {
            // we have only one unary operator -> negation
            auto e1 = expr.top();
            expr.pop();
            auto e = "!(" + e1 + ")";
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

std::string Interpreter::getVariables() {
    std::stringstream ss;
    for (auto& var : variableNames) {
        ss << var << " ";
    }
    return ss.str();
}

// Displays the truth table and returns the final result column.
std::vector<bool> Interpreter::evaluate() {
    return displayResultMatrix();
    // return evalPostfix();
    // return 0;
}
