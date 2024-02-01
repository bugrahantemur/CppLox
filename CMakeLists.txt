cmake_minimum_required(VERSION 3.27.8)
project(
  cpplox
  VERSION 0.1.0
  LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/b10fad38c4026a29ea6561ab15fc4818170d1c10.zip
)
FetchContent_MakeAvailable(googletest)

set(TARGET_SRC
    src/lox.cpp
    src/token/token.cpp
    src/scanner/scanner.cpp
    src/scanner/Error/Error.cpp
    src/scanner/cursor.cpp
    src/scanner/tokens.cpp
    src/parser/cursor.cpp
    src/parser/Error/Error.cpp
    src/parser/parser.cpp
    src/parser/expressions.cpp
    src/parser/statements.cpp
    src/interpreter/interpreter.cpp
    src/interpreter/Error/Error.cpp
    src/resolver/resolver.cpp
    src/resolver/Error/Error.cpp
)
add_executable(lox ${TARGET_SRC})

# TESTING
enable_testing()
add_executable(cpplox_test tests/tests.cpp)
target_link_libraries(cpplox_test GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(cpplox_test)

# PACKAGING
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
