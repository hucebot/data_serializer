cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(data_example)
find_package(data_serializer REQUIRED)
add_executable(example example.cpp)
target_compile_features(example PUBLIC cxx_std_14)
target_link_libraries(example PUBLIC data_serializer::data_serializer)





