cmake_minimum_required(VERSION 3.10)
project(LibraryManager CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

add_library(library_core
    src/Date.cpp
    src/Book.cpp
    src/Member.cpp
    src/Loan.cpp
    src/Library.cpp
)
target_include_directories(library_core PUBLIC include)

add_executable(library_manager src/main.cpp)
target_link_libraries(library_manager PRIVATE library_core)

enable_testing()
add_executable(library_tests tests/test_library.cpp)
target_link_libraries(library_tests PRIVATE library_core)
add_test(NAME LibraryTests COMMAND library_tests)
