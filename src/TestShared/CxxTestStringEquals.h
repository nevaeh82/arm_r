#ifndef CXXTESTSTRINGEQUALS_H
#define CXXTESTSTRINGEQUALS_H
#include <QString>

namespace CxxTest
{

inline bool equals( const QString&  x, const QString&  y)
{
	return x == y;
}
inline bool equals( QString&  x, QString&  y)
{
	return x == y;
}
inline bool equals( const QString&  x, QString&  y)
{
	return x == y;
}
inline bool equals( QString&  x, const QString&  y)
{
	return x == y;
}
}

//#ifdef CXXTEST_RUNNING
// This declaration is only activated when building a CxxTest test suite
#include <cxxtest/ValueTraits.h>
#include <stdio.h>

namespace CxxTest
{
   CXXTEST_TEMPLATE_INSTANTIATION
   class ValueTraits<QString>
   {
	   const QString& _m;

   public:
	   ValueTraits( const QString& m ) : _m( m ){ }
	  const char *asString() const { return _m.toStdString().c_str(); }
   };
}
//#endif // CXXTEST_RUNNING


#endif // CXXTESTSTRINGEQUALS_H
