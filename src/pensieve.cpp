#include "constants.h"
#include "interpreter.h"
#include "lexer.h"
#include "linenoise.h"
#include "stringutils.h"
#include <iostream>
#include <string.h>

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

        auto lexer = Lexer(input);
        auto tokens = lexer.tokenize();
        if (tokens.size() == 0) {
            // lexer encountered an error during tokenization, the expression is
            // invalid
            continue;
        }

        auto interpreter = Interpreter(tokens);
        if (debug) {
            std::cout << yellow("postfix:\t" + interpreter.getPostfix()) << "\n";
            std::cout << yellow("variables:\t" + interpreter.getVariables()) << "\n";
        }

        interpreter.evaluate();
        // std::cout << green(std::to_string(interpreter.evaluate())) << "\n";
    }

    linenoiseHistorySave(histFile);
    linenoiseHistoryFree();

    return 0;
}