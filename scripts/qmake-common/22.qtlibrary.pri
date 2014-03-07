# Add external library reference


defineReplace( qtlibrary ){
	libname=$$1
	version=$$2
	
	CONFIG(debug, release|debug) {
		libsuffix = d
	}
	linux-g++ {
		return(-l$${libname}$${libsuffix}$${version} )
	}
	win32-g++ {
		return( -l$${libname}$${libsuffix}$${version} )
	}
	win32-msvc2010 {
 		return( -l$${libname}$${libsuffix}$${version} )
	}
	error( platfrom not defined )
}

for( lib, QTLIBS): LIBS += $$qtlibrary( $$lib )
QTLIBS = 
