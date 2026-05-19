cmake_minimum_required(VERSION 4.3)

project(IMGLib
	LANGUAGES CXX
	VERSION 0.1
	DESCRIPTION "Image formats read and write library"
)

FILE(GLOB_RECURSE IMGLIB_SOURCES "src/*.cpp")
FILE(GLOB_RECURSE IMGLIB_HEADERS "include/*.hpp")

add_library(imglib STATIC
	${IMGLIB_SOURCES}
)
target_include_directories(imglib PUBLIC
	$<BUILD_INTERFACE:${IMGLib_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>
)

install(FILES ${IMGLIB_HEADERS} DESTINATION include)
install(TARGETS imglib EXPORT IMGLibTargets DESTINATION lib)
install(EXPORT IMGLibTargets DESTINATION lib/cmake/IMGLib)
install(FILES cmake/IMGLib.cmake DESTINATION lib/cmake/IMGLib)
