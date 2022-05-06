add_executable(data_to_csv data_to_csv.cpp)
target_compile_features(data_to_csv PUBLIC cxx_std_14)
target_link_libraries(data_to_csv PUBLIC data_serializer)





