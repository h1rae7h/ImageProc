cmake_minimum_required(VERSION 4.3)

project(ImageProc
	LANGUAGES CXX
	VERSION 0.1
	DESCRIPTION "Image formats processing library"
)

add_library(ImageProc INTERFACE)
target_include_directories(ImageProc INTERFACE
	$<BUILD_INTERFACE:${ImageProc_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>
)

install(DIRECTORY include/ImageProc DESTINATION include)
install(TARGETS ImageProc EXPORT ImageProcTargets)
install(EXPORT ImageProcTargets DESTINATION lib/cmake/ImageProc)
install(FILES cmake/ImageProcConfig.cmake DESTINATION lib/cmake/ImageProc)

add_executable(writetga
	src/examples/writetga.cpp
)
target_link_libraries(writetga PRIVATE ImageProc)
