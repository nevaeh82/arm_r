#
# Define target specification
# 

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

