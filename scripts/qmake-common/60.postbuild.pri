#
# Postbuild step.
#

exists( $$ProjectDir/product.mak ) {

	linux-g++ {
		QMAKE_POST_LINK  += $(MAKE) -f product.mak  -C $$ProjectDir  \
			DESTDIR_TARGET=$$DESTDIR/$$TARGET HOST=$$HOST MAKE_PLATFORM=$$SPEC BUILD_PROFILE=$$BUILD_PROFILE postbuild
	}

	win32-g++ {
		QMAKE_POST_LINK  += $(MAKE) -f product.mak  -C $$ProjectDir  \
			DESTDIR_TARGET=$$DESTDIR/$$TARGET HOST=$$HOST MAKE_PLATFORM=$$SPEC BUILD_PROFILE=$$BUILD_PROFILE postbuild
	}

	win32-msvc2010 {
		QMAKE_POST_LINK  += mingw32-make -f product.mak -C $$ProjectDir \
			DESTDIR_TARGET=$$DESTDIR/$$TARGET HOST=$$HOST MAKE_PLATFORM=$$SPEC BUILD_PROFILE=$$BUILD_PROFILE postbuild
	}
}