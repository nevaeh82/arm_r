project(Zaviruha)
cmake_minimum_required(VERSION 2.6.3)

set(PROJECT_NAME_GLOBAL ${PROJECT_NAME})

set(PROJECT_NAME_LOW "zaviruha")

# todo: rename SKYVERSION to smthng correct
if(NOT SKYVERSION)
	set(SKYVERSION "3.0.0")
endif()

set(VERSION_SUFFIX "GIT")

include(CheckIncludeFile)
include(CheckIncludeFiles)
include(CheckFunctionExists)
include(FindPkgConfig)
include(CheckCXXSourceCompiles)
find_package(PkgConfig)
find_package(Git)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(UsefulMacros)

option(MAKE_TESTS "Include tests in solution" ON)
option(INSTALL_RUNTIME_PATH "Install rpath" OFF)
option(INSTALL_RUNTIME_LIBS "Install runtime libs" ON)
option(USE_SYSLOG "Use syslog instead qxtlogger" OFF)
option(USE_COVERAGE "Run coverage for statistics" OFF)
option(CREATE_TR "Dynamical create qrc with qm files" ON)
option(CREATE_BUNDLE "Create bundle on install" ON)
option(USE_SHARED_OUTPUT "Place all build binaries on one common direcotory bin" ON)
option(NO_BACKTRACE "Build without backtrace" OFF)

if(NO_BACKTRACE)
	add_definitions(-DNO_BACKTRACE)
endif()

if(USE_SYSLOG)
	add_definitions(-DUSE_SYSLOG)
endif()

if(WIN32)
	add_definitions(-DUNICODE)
	add_definitions(-D_UNICODE)
endif()

if(USE_SHARED_OUTPUT)
	set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
	set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
	set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )

	set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin )
	set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin )
	set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin )

	set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin )
	set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin )
	set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin )
endif()

include(CommonInit)
include(CodeCoverage)

find_package(Threads REQUIRED)
find_package(Qt4 4.7.0 COMPONENTS QtCore QtNetwork QtSql QtGui QAxContainer REQUIRED)

SET(QT_USE_PHONON TRUE)

set(FULL_QT_VERSION "${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}")

find_package(Qxt REQUIRED)
find_package(Ipp REQUIRED)
find_package(Tbb REQUIRED)
find_package(GraphicsIpp REQUIRED)
find_package(Protobuf REQUIRED)
find_package(QtSingleApplication REQUIRED)
find_package(Solver REQUIRED)

if(SKYVERSION)
	string( REGEX REPLACE "\\..+" "" SKYVERSION_MAJOR ${SKYVERSION} )
	string( REGEX REPLACE "^[^.]+\\.([^.]+).*" "\\1" SKYVERSION_MINOR ${SKYVERSION} )
	string( REGEX REPLACE "^[^.]+\\.[^.]+\\.([^.]+).*" "\\1" SKYVERSION_PATCH ${SKYVERSION} )
endif()

if(INSTALL_RUNTIME_LIBS OR USE_SHARED_OUTPUT)
	set(runtime_libs)

	# collect runtime libs
	# todo: maybe it will be better to move the same actions below in function
	if(WIN32)
		if(QXT_LIBRARIES)
			foreach(lib ${QXT_LIBRARIES})
				get_filename_component(lib_dir ${lib} PATH)
				get_filename_component(lib_name ${lib} NAME_WE)
				string(REPLACE lib "" lib_name ${lib_name})
				file(GLOB libs ${lib_dir}/${lib_name}.dll)
				list(APPEND runtime_libs ${libs})
			endforeach()
		endif()

		if(GRAPHICSIPP_LIBRARIES)
			foreach(lib ${GRAPHICSIPP_LIBRARIES})
				get_filename_component(lib_dir ${lib} PATH)
				get_filename_component(lib_name ${lib} NAME_WE)
				string(REPLACE lib "" lib_name ${lib_name})
				file(GLOB libs ${lib_dir}/${lib_name}.dll)
				list(APPEND runtime_libs ${libs})
			endforeach()
		endif()

		if(IPP_LIBRARIES)
			list(GET IPP_LIBRARIES 0 lib)
			get_filename_component(lib_dir ${lib} PATH)
			list(APPEND runtime_libs
				${lib_dir}/libiomp5md.dll
				${lib_dir}/ippcore-7.0.dll
				${lib_dir}/ipps-7.0.dll
				${lib_dir}/ippsp8-7.0.dll)
		endif()

		if(TBB_LIBRARIES)
			foreach(lib ${TBB_LIBRARIES})
				get_filename_component(lib_dir ${lib} PATH)
				get_filename_component(lib_name ${lib} NAME_WE)
				string(REPLACE lib "" lib_name ${lib_name})
				file(GLOB libs ${lib_dir}/${lib_name}.dll)
				list(APPEND runtime_libs ${libs})
			endforeach()
		endif()

		if(SOLVER_LIBRARY)
			foreach(lib ${SOLVER_LIBRARY})
				get_filename_component(lib_dir ${lib} PATH)
				get_filename_component(lib_name ${lib} NAME_WE)
				string(REPLACE lib "" lib_name ${lib_name})
				file(GLOB libs ${lib_dir}/${lib_name}.dll)
				list(APPEND runtime_libs ${libs})
			endforeach()

			list(APPEND runtime_libs
				${lib_dir}/blas_win32_MT.dll
				${lib_dir}/lapack_win32_MT.dll)
		endif()
	endif()

	# install runtime libs
	if(INSTALL_RUNTIME_LIBS)
		install(FILES ${runtime_libs} DESTINATION ${LIBDIR})

		if(WIN32)
			if(NOT QT_PHONON_DS9_PLUGIN_RELEASE)
				status(WARNING "Qt plugin Phonon DS9 (release version) not found!")
			else()
				install(FILES ${QT_PHONON_DS9_PLUGIN_RELEASE} DESTINATION ${BINDIR}/plugins/phonon_backend)
			endif()
		endif()
	endif()

	# copy runtime libs to shared output directory for better debugging
	if(USE_SHARED_OUTPUT)
		add_custom_target(copy_runtime ALL COMMENT "Copy runtime libs to shared output directy")

		foreach(lib ${runtime_libs})
			get_filename_component(lib_name ${lib} NAME)
			add_custom_command(
				TARGET copy_runtime POST_BUILD
				COMMAND ${CMAKE_COMMAND} -E copy_if_different ${lib} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${lib_name})
		endforeach()
	endif()
endif()

add_subdirectory(src)

# settings for package target
if(WIN32)
	include(nsisParams.cmake)

	set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/win32/)
	set(CPACK_GENERATOR NSIS)
	set(CPACK_PACKAGE_VENDOR "STC")
	set(CPACK_PACKAGE_VERSION ${SKYVERSION})
	set(CPACK_NSIS_INSTALL_ROOT "C:")

	set(CPACK_PACKAGE_NAME "ZaviruhaR")
	set(CPACK_NSIS_INSTALLED_ICON_NAME "ZaviruhaClient.exe")
	list(APPEND CPACK_NSIS_CREATE_ICONS_EXTRA "
	CreateShortCut '$DESKTOP\\\\${PACKAGE_NAME_RUS} ${SKYVERSION}.lnk' '$INSTDIR\\\\ZaviruhaClient.exe'
	")
	list(APPEND CPACK_NSIS_DELETE_ICONS_EXTRA "
	Delete '$DESKTOP\\\\${PACKAGE_NAME_RUS} ${SKYVERSION}.lnk'
	")

	set( CPACK_NSIS_PACKAGE_NAME "${PACKAGE_NAME_RUS} ${SKYVERSION}" )
	set( CPACK_NSIS_DISPLAY_NAME "${CPACK_NSIS_PACKAGE_NAME}" )
	set(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${CPACK_PACKAGE_NAME})
	set(CPACK_PACKAGE_INSTALL_DIRECTORY "${CPACK_PACKAGE_VENDOR}\\\\${CPACK_PACKAGE_NAME}-${SKYVERSION}")
	set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}_${SKYVERSION}_Setup")
	set(CPACK_PACKAGE_INSTALL_REGISTRY_KEY ${CPACK_NSIS_DISPLAY_NAME})

	# enable uninstalling at first
	set( CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL "ON" )

	unset(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS)
	unset(NSIS_MENU_LINKS)
endif()

include(CPack)
