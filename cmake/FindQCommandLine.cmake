if( QCOMMANDLINE_INCLUDE_DIR AND QCOMMANDLINE_LIBRARY )
	set( QCOMMANDLINE_FIND_QUIETLY TRUE )
endif( QCOMMANDLINE_INCLUDE_DIR AND QCOMMANDLINE_LIBRARY )

string( TOUPPER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE_UP )

find_library( QCOMMANDLINE_LIBRARY QCommandLine${CMAKE_${CMAKE_BUILD_TYPE_UP}_POSTFIX} PATHS ${PROJECT_BASE_DIR}/redist/qcommandline/lib_${SPEC} )

set( QCOMMANDLINE_INCLUDE_DIR ${PROJECT_BASE_DIR}/redist/qcommandline/include )

include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( QCommandLine DEFAULT_MSG QCOMMANDLINE_LIBRARY QCOMMANDLINE_INCLUDE_DIR )

add_definitions( -DQCOMMANDLINE_STATIC )

mark_as_advanced( QCOMMANDLINE_INCLUDE_DIR QCOMMANDLINE_LIBRARY )
