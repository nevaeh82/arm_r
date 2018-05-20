#include "../../stdafx.h"
#include "strings.h"

#include <sstream>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>

namespace strings
{

std::wstring widen(const std::string& in, std::locale loc)
{
    std::wstring out(in.length(), 0);
    std::string::const_iterator i = in.begin(), ie = in.end();
    std::wstring::iterator j = out.begin();

    for( ; i!=ie; ++i, ++j )
        *j = std::use_facet< std::ctype<wchar_t> > (loc).widen(*i);

    return out;
}

void split(const std::string &s, char delim, std::vector<std::string> &elems)
{
    std::stringstream ss(s);
    std::string item;
    while(std::getline(ss, item, delim))
        if(!item.empty())
            elems.push_back(item);
}

// trim from start
std::string &ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

// trim from end
std::string &rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}

// trim from both ends
std::string &Trim(std::string &s) {
    return ltrim(rtrim(s));
}

//  если на работает, забыто #include <clocale> std::setlocale(LC_ALL, ""); при старте программы
std::wstring to_wstring(const std::string& str)
{
    size_t size;
    auto conv_result = mbstowcs_s(&size, nullptr, 0, str.c_str(), 0);
    if(conv_result == 0)
    {
        std::wstring result;
        if(size == 1)
            return result;
        result.resize(size);

        conv_result = mbstowcs_s(&size, &result[0], result.size(), str.c_str(), size);
        if(conv_result == 0)
        {
            if(result.size())
                result.resize(result.size()-1);
            return result;
        }
    }
    return std::wstring();
}


}  //  namespace strings
