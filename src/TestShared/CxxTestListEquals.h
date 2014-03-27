#ifndef CXXTESTLISTEQUALS_H
#define CXXTESTLISTEQUALS_H
#include <QList>
#include <QStringList>

namespace CxxTest
{

template <class T> inline bool equals( const QList<T>&  x, const QList<T>&  y)
{
	return x == y;
}
template <class T> inline bool equals( QList<T>&  x, QList<T>&  y)
{
	return x == y;
}
template <class T> inline bool equals( const QList<T>&  x, QList<T>&  y)
{
	return x == y;
}
template <class T> inline bool equals( QList<T>&  x, const QList<T>&  y)
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
	template <class T>
	class ValueTraits<QList<T> >
	{
		const QList<T>& _m;

	public:
		ValueTraits( const QList<T>& m ) : _m( m ){ }
		const char *asString() const { return "QList"; }
	};

	CXXTEST_TEMPLATE_INSTANTIATION
	class ValueTraits<QList<QString> >
	{
		const QStringList& _m;
		QString _s;

	public:
		ValueTraits( const QList<QString>& m ) : _m( m ), _s ("QList(" + _m.join(", ") + ")") {}

		const char *asString() const { return _s.toStdString().c_str(); }
	};

	CXXTEST_TEMPLATE_INSTANTIATION
	class ValueTraits<QStringList>
	{
		const QStringList& _m;
		QString _s;

	public:
		ValueTraits( const QStringList& m ) : _m( m ), _s ("QList(" + _m.join(", ") + ")") {}

		const char *asString() const { return _s.toStdString().c_str(); }
	};
}
//#endif // CXXTEST_RUNNING


#endif // CXXTESTLISTEQUALS_H
