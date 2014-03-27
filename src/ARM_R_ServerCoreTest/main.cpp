#include <QDir>
#include <QDebug>
#include <QTextStream>
#include <stdio.h>

#include "testsuite.cpp"

#if 0
int main()
{
    printf( "self: %s\n", __FILE__ );
    fflush( stdout );
    QTextStream  s(stdout);
//	s << "Project path: " << QFileInfo( __FILE__ ).dir().path() <<endl;
//	s << "Solution path: " << SolutionDir << endl;
//	s << "Version: " << VERSION << endl;
//	s << "ProjectName: " << ProjectName << endl;

	return 0;
}
#endif
