add_library(glad STATIC)
add_library(glad::glad ALIAS glad)

target_sources(glad
	PRIVATE
		"include/glad/glad.h"
		"include/KHR/khrplatform.h"
		"src/glad.c" 
)

target_include_directories(glad
	PUBLIC
		"${CMAKE_CURRENT_SOURCE_DIR}/include"
)
