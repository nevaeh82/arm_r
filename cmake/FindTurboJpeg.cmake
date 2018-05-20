if(TURBOJPEG_INCLUDE_DIR AND TURBOJPEG_LIBRARY)
	set(TURBOJPEG_FIND_QUIETLY TRUE)
endif(TURBOJPEG_INCLUDE_DIR AND TURBOJPEG_LIBRARY)

find_library(TURBOJPEG_LIBRARY turbojpeg
	PATHS ${PROJECT_BASE_DIR}/redist/turbojpeg/lib_${SPEC})

find_path(TURBOJPEG_INCLUDE_DIR turbojpeg.h
	PATHS ${PROJECT_BASE_DIR}/redist/turbojpeg/include)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(turbojpeg DEFAULT_MSG TURBOJPEG_LIBRARY TURBOJPEG_INCLUDE_DIR)

MARK_AS_ADVANCED(
	TURBOJPEG_INCLUDE_DIR
	TURBOJPEG_LIBRARY
)
