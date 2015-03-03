project(${PROJECT_NAME_GLOBAL}Common)

list(APPEND deps
	${PROJECT_NAME_GLOBAL}CISCommonLib
	QXT
)

setup_target_for_coverage(${PROJECT_NAME}_coverage ${PROJECT_NAME}Test coverage)
add_lib_static(${PROJECT_NAME} ${deps})
