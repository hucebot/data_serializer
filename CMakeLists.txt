cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(data_serializer)

include(CMakePackageConfigHelpers)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(DATA_SERIALIZER_VERSION 1.0.0)

find_package(Eigen3 REQUIRED)

add_library(data_serializer INTERFACE)
target_compile_features(data_serializer INTERFACE cxx_std_14)
target_link_libraries(data_serializer INTERFACE Eigen3::Eigen)

target_include_directories(data_serializer INTERFACE
		$<BUILD_INTERFACE:${data_serializer_BINARY_DIR}/include>
		$<BUILD_INTERFACE:${data_serializer_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
)

install(TARGETS data_serializer EXPORT targets
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include
)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_PREFIX}/include)

configure_package_config_file("cmake/data_serializer.cmake.in" "data_serializerConfig.cmake"
  INSTALL_DESTINATION "lib/cmake/data_serializer"
  PATH_VARS
)

install(EXPORT targets
  FILE data_serializerTargets.cmake
  NAMESPACE data_serializer::
  DESTINATION lib/cmake/data_serializer
)

write_basic_package_version_file("data_serializerConfigVersion.cmake"
  VERSION ${DATA_SERIALIZER_VERSION}
  COMPATIBILITY AnyNewerVersion
)

install(FILES 
	${data_serializer_BINARY_DIR}/data_serializerConfig.cmake 
	${data_serializer_BINARY_DIR}/data_serializerConfigVersion.cmake
        DESTINATION lib/cmake/data_serializer
)

