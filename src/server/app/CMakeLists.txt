project(${PROJECT_NAME_GLOBAL}Server)

list(APPEND deps ${PROJECT_NAME}Core)

if(NOT NO_BACKTRACE)
	list(APPEND deps WIN32_BT)
endif()

add_service(${PROJECT_NAME} ${deps})

# add installation for app runtime data files
install(FILES qt.conf DESTINATION ${BINDIR})

# copy runtime data to shared output dir
if(USE_SHARED_OUTPUT)
	add_custom_target(${PROJECT_NAME}_copy_runtime ALL COMMENT "Copy runtime libs to shared output directy for ${PROJECT_NAME}")

	add_custom_command(
		TARGET ${PROJECT_NAME}_copy_runtime POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/qt.conf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/qt.conf)
endif()

if(WIN32)
	# configure windows app version info
	configure_file(
		"${PROJECT_SOURCE_DIR}/version.h.in"
		"${PROJECT_BINARY_DIR}/version.h")

	# get path to plugins directory
	if(MSVC)
		set(plugins_spec "-${SPEC}")
	endif()
	set(qt_plugins_dir ${PROJECT_BASE_DIR}/redist/plugins/${QT_VERSION_API}${plugins_spec})

	# install batch for run server
	install(FILES ${PROJECT_SOURCE_DIR}/Server.bat DESTINATION ${BINDIR})

	if(INSTALL_RUNTIME_LIBS)
		# install Qt plugins
		install(DIRECTORY ${qt_plugins_dir}/ DESTINATION ${BINDIR}/plugins)

		# install Qt libs
		if(CREATE_BUNDLE)
			include(DeployQt4)
			install_qt4_executable(${PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX})
		endif()
	endif()

	# copy Qt plugins to runtime dir
	if(USE_SHARED_OUTPUT)
		add_custom_command(
			TARGET ${PROJECT_NAME}_copy_runtime POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_directory ${qt_plugins_dir} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/plugins)
	endif()
endif()
