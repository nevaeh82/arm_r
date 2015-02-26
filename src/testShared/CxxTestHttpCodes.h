#ifndef CXXTESTHTTPCODES_H
#define CXXTESTHTTPCODES_H
#include <cxxtest/TestSuite.h>
#include <HttpCodes.h>

namespace CxxTest
{

inline bool equals( const HttpCodes::Enum&  x, const HttpCodes::Enum&  y)
{
	return x == y;
}

}

// Declare value traits for the Answer enumeration
CXXTEST_ENUM_TRAITS(HttpCodes::Enum,
					CXXTEST_ENUM_MEMBER(HttpCodes::REDIRECT)
					CXXTEST_ENUM_MEMBER(HttpCodes::OK)
					CXXTEST_ENUM_MEMBER(HttpCodes::SERVER_ERROR)
					CXXTEST_ENUM_MEMBER(HttpCodes::NOT_CHANGED)
					CXXTEST_ENUM_MEMBER(HttpCodes::NOT_FOUND))


#endif // CXXTESTHTTPCODES_H
