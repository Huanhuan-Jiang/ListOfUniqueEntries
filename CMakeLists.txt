cmake_minimum_required(VERSION 3.29)
project(dequeofuniquetest VERSION 1.0)
# set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Enable necessary compile options
# add_compile_options(-std=c++23)

# cmake_policy(SET CMP0135 NEW)

set(GOOGLETEST_VERSION "1.14.0" CACHE STRING "Version of Google Test")

add_subdirectory(src)
add_subdirectory(thirdparty/googletest)

# add_executable(dequeofuniquetest tests/test_dequeofunique.cpp)

# Function to create an executable with a specific C++ standard
function(create_test_executable target_name cpp_standard)
    add_executable(${target_name} tests/test_dequeofunique.cpp)
    target_compile_features(${target_name} PRIVATE cxx_std_${cpp_standard})
    target_link_libraries(${target_name} PRIVATE
        GTest::gtest_main
        GTest::gmock_main
        containerofunique
    )
    enable_testing()
    include(GoogleTest)
    gtest_discover_tests(${target_name})
endfunction()

# Build executables for different C++ versions
create_test_executable(dequeofuniquetest_pre_cxx17 14)  # C++14 or earlier
create_test_executable(dequeofuniquetest_cxx17_to_cxx20 17)  # C++17 to C++20
create_test_executable(dequeofuniquetest_cxx20_to_cxx26 20)  # C++20 to C++26
create_test_executable(dequeofuniquetest_cxx26_and_later 23)  # C++26 and later

#target_link_libraries(dequeofuniquetest PRIVATE 
#    GTest::gtest_main 
#    GTest::gmock_main
#)

# target_link_libraries(dequeofuniquetest PRIVATE containerofunique)

# enable_testing()

# include(GoogleTest)
# gtest_discover_tests(dequeofuniquetest)
