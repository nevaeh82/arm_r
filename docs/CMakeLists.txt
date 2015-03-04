project(${PROJECT_NAME_GLOBAL}Docs)

if(WIN32)
	# add docs installation
	install(FILES Zaviruha_instruction.docx DESTINATION ${BINDIR})
endif()
