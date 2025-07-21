cmake_minimum_required(VERSION 3.16)
project(scheduleit LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS "src/*.cpp")
list(REMOVE_ITEM SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

# Build the main executable
add_executable(scheduleit src/main.cpp)
target_include_directories(scheduleit PRIVATE include)

add_library(scheduleitlib STATIC ${SRC_FILES})
target_include_directories(scheduleitlib PUBLIC include)
target_link_libraries(scheduleit PRIVATE scheduleitlib)

include(FetchContent)
FetchContent_Declare(
  catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.5.4
)
FetchContent_MakeAvailable(catch2)

file(GLOB_RECURSE TEST_FILES CONFIGURE_DEPENDS "tests/*Test.cpp")
add_executable(scheduleit_tests ${TEST_FILES})
target_link_libraries(scheduleit_tests PRIVATE scheduleitlib Catch2::Catch2WithMain)
target_include_directories(scheduleit_tests PRIVATE include)

include(CTest)
include(Catch)
enable_testing()
add_test(NAME scheduleit_tests COMMAND scheduleit_tests)
catch_discover_tests(scheduleit_tests)
