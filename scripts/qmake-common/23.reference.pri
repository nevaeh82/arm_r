contains(TEMPLATE, subdirs){ 
 	BUILD_ALL = $$(BUILD_ALL)
}


defineReplace( reference ){
	libname=$$1
	!contains(BUILD_ALL,true){ 
		REFERENCES += $$libname
	}
	else {
		message(Import reference ignored by Total build )
	}
	return( $$library( $$libname, $$version_maj()) )
}


defineReplace( staticReference ){
	libname=$$1
	!contains(BUILD_ALL,true){ 
		REFERENCES += $$libname
	}
	else {
		message(Import reference ignored by Total build )
	}
	return( $$staticLibrary( $$libname) )
}




# required for building by reference


CONFIG(debug, debug|release) {
 BUILD_PROFILE = debug
}
CONFIG(release, debug|release) {
 BUILD_PROFILE = release
}


