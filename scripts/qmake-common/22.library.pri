# Add external library reference


defineReplace( library ){
	libname=$$1
	version=$$2
	
	CONFIG(debug, release|debug) {
		libsuffix = d
	}
	linux-g++ {
		return(-l$${libname}$${libsuffix} )
	}
	win32-g++ {
		return( -l$${libname}$${libsuffix}$${version} )
	}
	win32-msvc2010 {
 		return( -l$${libname}$${libsuffix}$${version} )
	}
	error( platfrom not defined )
}

defineReplace( staticLibrary ){
	libname=$$1
	
	CONFIG(debug, release|debug) {
		libsuffix = d
	}
	linux-g++ {
		return(-l$${libname}$${libsuffix} )
	}
	win32-g++ {
		return( -l$${libname}$${libsuffix} )
	}
	win32-msvc2010 {
 		return( -l$${libname}$${libsuffix} )
	}
	error( platfrom not defined )
}
