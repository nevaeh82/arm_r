
linux-g++ {
    MACHTYPE=$$system( echo $MACHTYPE )
    contains( MACHTYPE, i686-vniins-linux-gnu ) {
	HOST=msvs
 	SPEC=msvs
    }
    else {
	contains( MACHTYPE, x86_64-pc-linux-gnu ) {
	    HOST=astra
	    SPEC=astra
	}
	else {
	    HOST=linux
	    SPEC=linux
	}
    }  
}

win32-g++ { 
	HOST=win32
 	SPEC=mingw
}
win32-msvc2010 {
	HOST=win32
 	SPEC=msvc
}



defineReplace( pwgisLibrary ){
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


isEmpty(DESTDIR) {
	CONFIG(debug, debug|release) {
		DESTDIR = $$_PRO_FILE_PWD_/debug
	}
	CONFIG(release, debug|release) {
		DESTDIR = $$_PRO_FILE_PWD_/release
	}
}       	


defineTest( pwgisDeployDependencies  ) {

	libs = $$1
	destdir = $$DESTDIR

win32:	system( mkdir \"$$replace(destdir,/,\\)\")
unix:	system( mkdir \"$$destdir\")



    LIBDIRS = $$find(libs,-L)
    MYLIBS = $$find(libs,-l)

    LIBDIRS = $$replace(LIBDIRS, -L,"")
    MYLIBS = $$replace(MYLIBS, -l,)
    #
    message(prepare deps $$(libs)... )

    for(libdir, LIBDIRS):{
        for(mylib, MYLIBS):{
             exists("$${libdir}/$${mylib}.dll"):{
                # message(copy library $${libdir}/$${mylib}.dll -> $$destdir)
	        system(cp -uv \"$${libdir}/$${mylib}.dll\" \"$$destdir/$${mylib}.dll\")
            }
        }
    }
}

