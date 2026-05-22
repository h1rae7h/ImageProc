cmake_minimum_required(VERSION 4.3)

project(ImageProc
	LANGUAGES CXX
	VERSION 0.2
	DESCRIPTION "Image formats processing library"
)

FILE(GLOB IMAGEPROCLIB_SOURCES "src/ImageProc/*.cpp")

add_library(ImageProc STATIC
	${IMAGEPROCLIB_SOURCES}
)
target_include_directories(ImageProc PUBLIC
	$<BUILD_INTERFACE:${ImageProc_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>
)

install(DIRECTORY include/ImageProc DESTINATION include)
install(TARGETS ImageProc EXPORT ImageProcTargets DESTINATION lib)
install(EXPORT ImageProcTargets DESTINATION lib/cmake/ImageProc)
install(FILES cmake/ImageProcConfig.cmake DESTINATION lib/cmake/ImageProc)

add_executable(writetga
	src/examples/writetga.cpp
)
target_link_libraries(writetga PRIVATE ImageProc)

add_executable(writebmp
	src/examples/writebmp.cpp
)
target_link_libraries(writebmp PRIVATE ImageProc)
