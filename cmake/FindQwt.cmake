if(QWT_INCLUDE_DIR AND QWT_LIBRARY)
	set(QWT_FIND_QUIETLY TRUE)
endif(QWT_INCLUDE_DIR AND QWT_LIBRARY)

set(QWT_PATH qwt)

find_library(QWT_LIBRARY
		NAMES
			qwt${BICYCLE_DEBUG_POSTFIX}
			PATHS ${PROJECT_BASE_DIR}/redist/${QWT_PATH}/${FULL_QT_VERSION}/lib_${SPEC}
		)

find_path(QWT_INCLUDE_DIR
			qwt.h
			PATHS ${PROJECT_BASE_DIR}/redist/${QWT_PATH}/${FULL_QT_VERSION}/include
		)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Qwt DEFAULT_MSG QWT_LIBRARY QWT_INCLUDE_DIR)

MARK_AS_ADVANCED (
	QWT_LIBRARY
	QWT_INCLUDE_DIR
)
