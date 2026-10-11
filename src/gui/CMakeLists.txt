add_library(gui STATIC gui.cpp layout.cpp input.cpp)
target_include_directories(gui PUBLIC ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(gui PRIVATE SDL3::SDL3 engine)
