#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include <algorithm>
#include <string>

// trim from start (in place)
void ltrim(std::string& s);

// trim from end (in place)
void rtrim(std::string& s);

// trim string from both ends
void trim(std::string& s);

#endif // STRINGUTILS_H
