#ifndef CXXTESTMAPEQUALS_H
#define CXXTESTMAPEQUALS_H
#include <QString>
#include <QMap>

namespace CxxTest
{

template <class Key, class T> inline bool equals( const QMap<Key, T>&  x, const QMap<Key, T>&  y)
{
	return x == y;
}
template <class Key, class T> inline bool equals( QMap<Key, T>&  x, QMap<Key, T>&  y)
{
	return x == y;
}
template <class Key, class T> inline bool equals( const QMap<Key, T>&  x, QMap<Key, T>&  y)
{
	return x == y;
}
template <class Key, class T> inline bool equals( QMap<Key, T>&  x, const QMap<Key, T>&  y)
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
	template <class Key, class T>
	class ValueTraits<QMap<Key, T> >
	{
		const QMap<Key, T>& _m;
		const QString _s;

	public:
		ValueTraits( const QMap<Key, T>& m ) : _m( m ), _s( "QMap(..)" ){ }
		const char *asString() const { return _s.toStdString().c_str(); }
	};
}
//#endif // CXXTEST_RUNNING


#endif // CXXTESTMAPEQUALS_H
