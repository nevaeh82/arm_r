project(${PROJECT_NAME_GLOBAL}ViewTest)

list(APPEND deps
	${PROJECT_NAME_GLOBAL}ViewCore
)

add_bicycle_test(${PROJECT_NAME} ${deps})
