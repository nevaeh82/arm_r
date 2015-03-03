project(${PROJECT_NAME_GLOBAL}Server)

list(APPEND deps
	${PROJECT_NAME}Core
)

if(NOT NO_BACKTRACE)
	list(APPEND deps WIN32_BT)
endif()

add_service(${PROJECT_NAME} ${deps})

# add installation for app runtime data files
set(runtime_data_dirs audio DB RDP RDS Rpc Tabs TCP)
install(DIRECTORY runtime_data_dirs DESTINATION ${BINDIR})
install(FILES qt.conf DESTINATION ${BINDIR})

# copy runtime data to shared output dir
if(USE_SHARED_OUTPUT)
	add_custom_target(${PROJECT_NAME}_copy_runtime ALL COMMENT "Copy runtime libs to shared output directy for ${PROJECT_NAME}")

	add_custom_command(
		TARGET ${PROJECT_NAME}_copy_runtime POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/qt.conf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/qt.conf)

	foreach(dir ${runtime_data_dirs})
		add_custom_command(
			TARGET ${PROJECT_NAME}_copy_runtime POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/${dir} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${dir})
	endforeach()
endif()

if(WIN32)
	configure_file(
		"${PROJECT_SOURCE_DIR}/version.h.in"
		"${PROJECT_BINARY_DIR}/version.h")

	if(MSVC)
		set(plugins_spec "-${SPEC}")
	endif()
	set(qt_plugins_dir ${PROJECT_BASE_DIR}/redist/plugins/${FULL_QT_VERSION}${plugins_spec})
	message( ${qt_plugins_dir} )

	install(FILES ${PROJECT_SOURCE_DIR}/Server.bat DESTINATION ${BINDIR})
	install(DIRECTORY ${qt_plugins_dir} DESTINATION ${BINDIR})

	set(NSIS_EXE_FOR_LINKS ${NSIS_EXE_FOR_LINKS} "${PROJECT_NAME}" CACHE STRING "" FORCE)

	if(INSTALL_RUNTIME_LIBS AND CREATE_BUNDLE)
		if(SPEC STREQUAL "mingw")
			set(gp_tool "objdump")
		endif()

		include(DeployQt4)
		install_qt4_executable(${PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX})
	endif()

	if(USE_SHARED_OUTPUT)
		add_custom_command(
			TARGET ${PROJECT_NAME}_copy_runtime POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_directory ${qt_plugins_dir} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/plugins)
	endif()
endif()
