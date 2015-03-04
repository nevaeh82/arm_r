project(${PROJECT_NAME_GLOBAL}Docs)

if(WIN32)
	# add docs installation
	install(FILES Zaviruha_instruction.docx DESTINATION ${BINDIR})

	# settings for cpack
	include(nsis.cmake)

	# add link to docs in Windows menu
	set(CPACK_NSIS_MENU_LINKS ${CPACK_NSIS_MENU_LINKS}
		"$INSTDIR\\\\Zaviruha_instruction.docx" "${PROJECT_NAME_RUS}"
	)
endif()
