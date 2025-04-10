#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

// Console colors
extern const std::string COLOR_RED;
extern const std::string COLOR_GREEN;
extern const std::string COLOR_YELLOW;
extern const std::string COLOR_BLUE;
extern const std::string COLOR_PURPLE;
extern const std::string COLOR_CYAN;
extern const std::string COLOR_RESET;

// Color utility functions
std::string purple(std::string s);
std::string cyan(std::string s);
std::string green(std::string s);
std::string yellow(std::string s);

#endif // CONSTANTS_H
