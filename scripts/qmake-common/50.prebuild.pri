#
# Prebuild step.
#

exists( $$ProjectDir/product.mak ) {
   defineTest( applyReferences ) {
	HEADERS = $$unique(HEADERS)

	win32-g++ {
		prebuild.commands = $(MAKE) -f product.mak  -C $$ProjectDir \
			MAKE_DESTDIR_TARGET=$$DESTDIR/$$TARGET  \
			HOST=$$HOST \
			MAKE_PLATFORM=$$SPEC \
			HEADERS=\"$$HEADERS\" \
			REFERENCES=\"$$REFERENCES\" \
			BUILD_PROFILE=$$BUILD_PROFILE\
			prebuild
			
		QMAKE_EXTRA_TARGETS += prebuild
		PRE_TARGETDEPS += prebuild

		.NOTPARALLEL.depends = prebuild
		QMAKE_EXTRA_TARGETS += .NOTPARALLEL
		
		PRE_TARGETDEPS      = $$unique(PRE_TARGETDEPS)
		QMAKE_EXTRA_TARGETS = $$unique(QMAKE_EXTRA_TARGETS)
		
	}
	linux-g++ {
		prebuild.commands = $(MAKE) -f product.mak -C $$ProjectDir \
			MAKE_DESTDIR_TARGET=$$DESTDIR/$$TARGET \
			HOST=$$HOST \
			MAKE_PLATFORM=$$SPEC  \
			HEADERS=\"$$HEADERS\" \
			REFERENCES=\"$$REFERENCES\" \
			BUILD_PROFILE=$$BUILD_PROFILE\
			prebuild

		QMAKE_EXTRA_TARGETS += prebuild
		PRE_TARGETDEPS += prebuild

		.NOTPARALLEL.depends = prebuild
		QMAKE_EXTRA_TARGETS += .NOTPARALLEL
		
		PRE_TARGETDEPS      = $$unique(PRE_TARGETDEPS)
		QMAKE_EXTRA_TARGETS = $$unique(QMAKE_EXTRA_TARGETS)

	}

	win32-msvc2010 {
		contains( TEMPLATE, vclib ) | contains( TEMPLATE, vcapp ) {
		QMAKE_PRE_BUILD = mingw32-make -f product.mak -C $$ProjectDir \
			MAKE_DESTDIR_TARGET=$$DESTDIR/$$TARGET \
			HOST=$$HOST \
			MAKE_PLATFORM=$$SPEC  \
			HEADERS=\"$$HEADERS\" \
			REFERENCES=\"$$REFERENCES\" \
			BUILD_PROFILE=$$BUILD_PROFILE\
			prebuild
		}
		else {
		prebuild.commands = mingw32-make -f product.mak -C $$ProjectDir \
			MAKE_DESTDIR_TARGET=$$DESTDIR/$$TARGET \
			HOST=$$HOST \
			MAKE_PLATFORM=$$SPEC  \
			HEADERS=\"$$HEADERS\" \
			REFERENCES=\"$$REFERENCES\" \
			BUILD_PROFILE=$$BUILD_PROFILE\
			prebuild

		QMAKE_EXTRA_TARGETS += prebuild
		PRE_TARGETDEPS += prebuild

		.NOTPARALLEL.depends = prebuild
		QMAKE_EXTRA_TARGETS += .NOTPARALLEL
		
		PRE_TARGETDEPS      = $$unique(PRE_TARGETDEPS)
		QMAKE_EXTRA_TARGETS = $$unique(QMAKE_EXTRA_TARGETS)
		}
		
	}
	export( prebuild )
	export( prebuild.commands )
	export( QMAKE_EXTRA_TARGETS )
	export( PRE_TARGETDEPS )
	export( QMAKE_PRE_BUILD)
  }
  applyReferences()
}

