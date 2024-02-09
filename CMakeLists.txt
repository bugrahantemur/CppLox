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
    "src/Lox.cpp"
    "src/Error/Report/Report.cpp"
    "src/Interpreter/Interpreter.cpp"
    "src/Interpreter/Statements/Execute.cpp"
    "src/Interpreter/Expressions/Call/Call.cpp"
    "src/Interpreter/Expressions/Equality/Equality.cpp"
    "src/Interpreter/Expressions/Class/Class.cpp"
    "src/Interpreter/Expressions/Evaluate.cpp"
    "src/Interpreter/Error/Error.cpp"
    "src/Interpreter/Utils/Truth/Truth.cpp"
    "src/Parser/Cursor/Cursor.cpp"
    "src/Parser/Error/Error.cpp"
    "src/Parser/Parser.cpp"
    "src/Parser/Expressions/Expressions.cpp"
    "src/Parser/Statements/Statements.cpp"
    "src/Reader/Error/Error.cpp"
    "src/Reader/Reader.cpp"
    "src/Resolver/Resolver.cpp"
    "src/Resolver/Error/Error.cpp"
    "src/Scanner/Scanner.cpp"
    "src/Scanner/Error/Error.cpp"
    "src/Scanner/Cursor/Cursor.cpp"
    "src/Scanner/Tokens/Tokens.cpp"
    "src/Types/Tokens/Token.cpp"
    "src/Types/Environment/Environment.cpp"
)

add_executable(lox ${TARGET_SRC})

# TESTING
enable_testing()
add_executable(cpplox_test tests/tests.cpp src/Reader/Error/Error.cpp src/Reader/Reader.cpp)
target_link_libraries(cpplox_test GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(cpplox_test)

# PACKAGING
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
