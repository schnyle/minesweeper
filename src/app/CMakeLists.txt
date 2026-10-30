add_library(app STATIC app.cpp)
target_include_directories(app PUBLIC ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(app PUBLIC gui engine SDL3::SDL3)
