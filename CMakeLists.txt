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

set(COMMON_SRC
  "src/Common/Error/Report/Report.cpp"
  "src/Common/Types/Tokens/Token.cpp"
  "src/Common/Reader/Reader.cpp"
  "src/Common/Reader/Error/Error.cpp"
  "src/Common/Scanner/Scanner.cpp"
  "src/Common/Scanner/Cursor/Cursor.cpp"
  "src/Common/Scanner/Error/Error.cpp"
  "src/Common/Scanner/Tokens/Tokens.cpp"
)
set(TREEWALK_SRC
  ${COMMON_SRC}
  "src/TreeWalk/Lox.cpp"
  "src/TreeWalk/Builtins/Functions/Clock/Clock.cpp"
  "src/TreeWalk/Interpreter/Interpreter.cpp"
  "src/TreeWalk/Interpreter/Statements/Execute.cpp"
  "src/TreeWalk/Interpreter/Expressions/Call/Call.cpp"
  "src/TreeWalk/Interpreter/Expressions/Equality/Equality.cpp"
  "src/TreeWalk/Interpreter/Expressions/Class/Class.cpp"
  "src/TreeWalk/Interpreter/Expressions/Evaluate.cpp"
  "src/TreeWalk/Interpreter/Error/Error.cpp"
  "src/TreeWalk/Interpreter/Utils/Truth/Truth.cpp"
  "src/TreeWalk/Parser/Cursor/Cursor.cpp"
  "src/TreeWalk/Parser/Error/Error.cpp"
  "src/TreeWalk/Parser/Parser.cpp"
  "src/TreeWalk/Parser/Expressions/Expressions.cpp"
  "src/TreeWalk/Parser/Statements/Statements.cpp"
  "src/TreeWalk/Resolver/Resolver.cpp"
  "src/TreeWalk/Resolver/Error/Error.cpp"
  "src/TreeWalk/Types/Environment/Environment.cpp"
)
set(BYTECODE_SRC
  ${COMMON_SRC}
  "src/Bytecode/Lox.cpp"
)

add_executable(twlox ${TREEWALK_SRC})
add_executable(bclox ${BYTECODE_SRC})

# TESTING
enable_testing()
add_executable(cpplox_test unit_tests/tests.cpp src/Common/Reader/Error/Error.cpp src/Common/Reader/Reader.cpp)
target_link_libraries(cpplox_test GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(cpplox_test)

# PACKAGING
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
