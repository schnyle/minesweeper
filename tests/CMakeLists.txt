include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/releases/download/v1.18.0/googletest-1.18.0.tar.gz
  URL_HASH SHA256=6e3191c1455468b3fc35a417fb565c1c5071aee1b7e7f85e30cf48a98d37d8b5
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(engine_test engine/engine_test.cpp)
target_link_libraries(engine_test PRIVATE engine GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(engine_test)
