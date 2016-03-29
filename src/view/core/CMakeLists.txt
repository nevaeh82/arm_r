project(${PROJECT_NAME_GLOBAL}ViewCore)

list(APPEND deps
	${PROJECT_NAME_GLOBAL}CISCommonLib
	${PROJECT_NAME_GLOBAL}Common
	PROTOBUF
	GraphicsIpp
	IPP
	TBB
	QtSingleApplication
	Solver
)

string(REPLACE Core "" PROJECT_NAME_COV ${PROJECT_NAME})
setup_target_for_coverage(${PROJECT_NAME}_coverage ${PROJECT_NAME_COV}Test coverage)

add_lib_static(${PROJECT_NAME} ${deps})
