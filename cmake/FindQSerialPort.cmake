if(QSERIAL_INCLUDE_DIR AND QSERIAL_LIBRARY)
	set(QSERIAL_FIND_QUIETLY TRUE)
endif(QSERIAL_INCLUDE_DIR AND QSERIAL_LIBRARY)

find_library(QSERIAL_LIBRARY QtSerialPort${BICYCLE_DEBUG_POSTFIX}
    PATHS ${PROJECT_BASE_DIR}/redist/qtserialport/${FULL_QT_VERSION}/lib_${SPEC})

find_path(QSERIAL_INCLUDE_DIR QtSerialPort/QSerialPort
	PATHS ${PROJECT_BASE_DIR}/redist/qtserialport/${FULL_QT_VERSION}/include)

MESSAGE(${PROJECT_BASE_DIR}/redist/qtserialport/${FULL_QT_VERSION}/include)

if (WIN32)
	set(QSERIAL_LIBRARY ${QSERIAL_LIBRARY} -lsetupapi)
elseif(UNIX)
	if (NOT UDEV_LIBRARY)
		find_library(UDEV_LIBRARY udev)
	endif()
	set(QSERIAL_LIBRARY ${QSERIAL_LIBRARY} ${UDEV_LIBRARY})
endif ()

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(QSerialPort DEFAULT_MSG QSERIAL_LIBRARY QSERIAL_INCLUDE_DIR)

MARK_AS_ADVANCED(
	QSERIAL_INCLUDE_DIR
	QSERIAL_LIBRARY
)
