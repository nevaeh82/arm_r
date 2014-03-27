#ifndef MODULES_CORE_STRINGS_H_
#define MODULES_CORE_STRINGS_H_ 2

#include <string>
#include <vector>

namespace strings
{

std::wstring widen(const std::string& in, std::locale loc = std::locale());
void split(const std::string &s, char delim, std::vector<std::string> &elems);
std::string &Trim(std::string &s);

//  если на работает, забыто #include <clocale> std::setlocale(LC_ALL, ""); при старте программы
std::wstring to_wstring(const std::string& str);

}  //  namespace strings

#endif