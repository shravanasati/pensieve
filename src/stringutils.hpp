#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include <algorithm>
#include <string>
#include <vector>

// trim from start (in place)
void ltrim(std::string& s);

// trim from end (in place)
void rtrim(std::string& s);

// trim string from both ends
void trim(std::string& s);

std::vector<std::string> split(const std::string& s, char delimiter);

#endif // STRINGUTILS_H
