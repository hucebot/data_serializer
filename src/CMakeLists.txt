add_executable(data_to_csv data_to_csv.cpp)
target_compile_features(data_to_csv PUBLIC cxx_std_14)
target_link_libraries(data_to_csv PUBLIC data_serializer)

add_executable(example example.cpp)
target_compile_features(example PUBLIC cxx_std_14)
target_link_libraries(example PUBLIC data_serializer)

if(${CMAKE_CXX_STANDARD} MATCHES "17")
    add_executable(recursive_data_to_csv recursive_data_to_csv.cpp)
    target_compile_features(recursive_data_to_csv PUBLIC cxx_std_17)
    target_link_libraries(recursive_data_to_csv PUBLIC data_serializer)
endif()


