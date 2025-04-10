#include "constants.hpp"
#include "interpreter.hpp"
#include "lexer.hpp"
#include "linenoise.h"
#include "stringutils.hpp"
#include <iostream>
#include <string.h>
#include <vector>

template <typename T>
bool compareVectors(const std::vector<T>& v1, const std::vector<T>& v2) {
    if (v1.size() != v2.size()) {
        return false;
    }
    for (size_t i = 0; i < v1.size(); i++) {
        if (v1[i] != v2[i]) {
            return false;
        }
    }
    return true;
}

/* * LINENOISE CONFIG * */

static const char* examples[] = {"/debug", "/q", "exit", "quit", NULL};

void completionHook(char const* prefix, linenoiseCompletions* lc) {
    size_t i;

    for (i = 0; examples[i] != NULL; ++i) {
        if (strncmp(prefix, examples[i], strlen(prefix)) == 0) {
            linenoiseAddCompletion(lc, examples[i]);
        }
    }
}

int main(int argc, char const* argv[]) {
    linenoiseInstallWindowChangeHandler();
    linenoiseSetCompletionCallback(completionHook);
    linenoiseHistorySetMaxLen(1000);
    const char* histFile = "~/.pensieve_history";
    linenoiseHistoryLoad(histFile);
    std::cout << std::boolalpha;

    std::cout << purple("exit OR quit OR /q to exit") << '\n';

    auto debug = false;
    while (true) {
        char* result = linenoise(cyan("pensieve > ").c_str());
        if (result == NULL) {
            continue;
        }
        if (*result == '\0') {
            free(result);
            break;
        }

        linenoiseHistoryAdd(result);

        std::string input(result);
        // std::cout << cyan("pensieve > ");
        // std::getline(std::cin, input);
        trim(input);
        std::transform(input.begin(), input.end(), input.begin(),
                       [](unsigned char c) { return std::tolower(c); });

        if (input == "") {
            continue;
        }

        if (input == "quit" || input == "exit" || input == "/q") {
            std::cout << purple("bye") << std::endl;
            break;
        }

        if (input == "/debug") {
            debug = !debug;
            std::cout << purple("debug mode ")
                      << purple((debug) ? "enabled" : "disabled") << '\n';
            continue;
        }

        auto expressions = split(input, ',');
        std::vector<std::vector<bool>> results; // Remove size initialization

        for (auto& expr : expressions) {
            trim(expr); // Add trim to remove whitespace from split expressions
            auto lexer = Lexer(expr);
            auto tokens = lexer.tokenize();
            if (tokens.size() == 0) {
                // lexer encountered an error during tokenization, the
                // expression is invalid
                continue;
            }

            auto interpreter = Interpreter(tokens);
            if (debug) {
                std::cout << yellow("postfix:\t" + interpreter.getPostfix())
                          << "\n";
                std::cout << yellow("variables:\t" + interpreter.getVariables())
                          << "\n";
            }

            auto result = interpreter.evaluate();
            results.push_back(result);
        }

        if (results.size() > 1) {
            bool same = true;
            const auto& firstResult = results[0];

            for (size_t i = 1; i < results.size(); i++) {
                if (!compareVectors(results[i], firstResult)) {
                    same = false;
                    break;
                }
            }

            if (same) {
                std::cout
                    << green("All these expressions are logically equivalent")
                    << std::endl;
            } else {
                std::cout
                    << red("All these expressions are NOT logically equivalent")
                    << std::endl;
            }
        }
    }

    linenoiseHistorySave(histFile);
    linenoiseHistoryFree();

    return 0;
}