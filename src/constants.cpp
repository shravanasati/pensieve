#include "constants.h"

const std::string COLOR_RED = "\033[31m";
const std::string COLOR_GREEN = "\033[32m";
const std::string COLOR_YELLOW = "\033[33m";
const std::string COLOR_BLUE = "\033[34m";
const std::string COLOR_PURPLE = "\033[35m";
const std::string COLOR_CYAN = "\033[36m";
const std::string COLOR_RESET = "\033[0m";

std::string purple(std::string s) { return COLOR_PURPLE + s + COLOR_RESET; }
std::string cyan(std::string s) { return COLOR_CYAN + s + COLOR_RESET; }
std::string green(std::string s) { return COLOR_GREEN + s + COLOR_RESET; }
std::string yellow(std::string s) { return COLOR_YELLOW + s + COLOR_RESET; }
