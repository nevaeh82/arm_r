#ifndef __cxxtest__VerbosePrinter_h__
#define __cxxtest__VerbosePrinter_h__

//
// The ErrorPrinter is a simple TestListener that
// just prints "OK" if everything goes well, otherwise
// reports the error in the format of compiler messages.
// The ErrorPrinter uses std::cout
//

#include <cxxtest/Flags.h>

#ifndef _CXXTEST_HAVE_STD
#   define _CXXTEST_HAVE_STD
#endif // _CXXTEST_HAVE_STD

#include <cxxtest/ErrorFormatter.h>
#include <cxxtest/StdValueTraits.h>

#ifdef _CXXTEST_OLD_STD
#   include <iostream.h>
#else // !_CXXTEST_OLD_STD
#   include <iostream>
#endif // _CXXTEST_OLD_STD

#include <QDateTime>
#include <QDebug>

#if QT_VERSION < 0x070000
#define QDATETIME_NOWMSECS()  (QDateTime::currentDateTime().toTime_t() * 1000)
#else 
#define QDATETIME_NOWMSECS()  (QDateTime::currentMSecsSinceEpoch())
#endif    

namespace CxxTest
{
class VerbosePrinter : public ErrorFormatter
{
public:
	VerbosePrinter( CXXTEST_STD(ostream) &o = CXXTEST_STD(cout), const char *preLine = ":", const char *postLine = "" ) :
		ErrorFormatter( new Adapter(o), preLine, postLine ) {}
	virtual ~VerbosePrinter() { delete outputStream(); }

	qint64 started;

	void enterTest( const TestDescription &test )
	{
		ErrorFormatter::enterTest( test );
		(*outputStream()) << "Test: " << test.suiteName() << "::" <<test.testName() << "..." << endl;
		started  = QDATETIME_NOWMSECS();

		//notify test log about test.
		qDebug() << QString("Test: %1::%2...\n" ).arg(test.suiteName()).arg(test.testName());
	}
	void leaveTest( const TestDescription &test )
	{
		ErrorFormatter::leaveTest( test );
		if ( !tracker().testFailed() ) {
			(*outputStream()) << "Test: " << test.suiteName() << "::" <<test.testName() << "...OK";
		}
		else {
			(*outputStream()) << "Test: " << test.suiteName() << "::" <<test.testName() << "...FAILED";
		}

		qint64 msecSpan = QDATETIME_NOWMSECS() - started;
		if ( msecSpan >  50 ) {
			(*outputStream()) << "  TEST OVERTIMED " << int((double)msecSpan / 1000.) << " secs";
		}

		(*outputStream())  << endl;
	}
private:
	static void endl( OutputStream &o )
	{
		OutputStream::endl( o );
	}

	class Adapter : public OutputStream
	{
		CXXTEST_STD(ostream) &_o;
	public:
		Adapter( CXXTEST_STD(ostream) &o ) : _o(o) {}
		void flush() { _o.flush(); }
		OutputStream &operator<<( const char *s ) { _o << s; return *this; }
		OutputStream &operator<<( Manipulator m ) { return OutputStream::operator<<( m ); }
		OutputStream &operator<<( unsigned i )
		{
			char s[1 + 3 * sizeof(unsigned)];
			numberToString( i, s );
			_o << s;
			return *this;
		}
	};
};
}

#endif // __cxxtest__ErrorPrinter_h__
