cmake_minimum_required(VERSION 3.29)
project(containerofuniquetest VERSION 1.0)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(GOOGLETEST_VERSION "1.14.0" CACHE STRING "Version of Google Test")

add_subdirectory(src)
add_subdirectory(thirdparty/googletest)

add_executable(dequeofuniquetest tests/test_dequeofunique.cpp)
add_executable(vectorofuniquetest tests/test_vectorofunique.cpp)

target_link_libraries(dequeofuniquetest vectorofuniquetest PRIVATE 
    GTest::gtest_main 
    GTest::gmock_main
)

target_link_libraries(dequeofuniquetest vectorofuniquetest PRIVATE 
containerofunique)

enable_testing()

include(GoogleTest)
gtest_discover_tests(dequeofuniquetest)

# Build executables for different C++ versions
create_test_executable(dequeofuniquetest_cxx14 14)  
create_test_executable(dequeofuniquetest_cxx17 17)  
create_test_executable(dequeofuniquetest_cxx20 20)  
create_test_executable(dequeofuniquetest_cxx23 23)