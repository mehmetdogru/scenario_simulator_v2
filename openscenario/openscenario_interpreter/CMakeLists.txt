cmake_minimum_required(VERSION 3.16.3) # Ubuntu 20.04 default CMake version

project(openscenario_interpreter)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake_auto REQUIRED)

ament_auto_find_build_dependencies()

# ------------------------------------------------------------------------------
#  libopenscenario_interpreter
# ------------------------------------------------------------------------------

file(GLOB ${PROJECT_NAME}_POSIX_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/posix/*.cpp)
file(GLOB ${PROJECT_NAME}_SYNTAX_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/syntax/*.cpp)
file(GLOB ${PROJECT_NAME}_UTILITY_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/utility/*.cpp)

ament_auto_add_library(${PROJECT_NAME} SHARED
  ${${PROJECT_NAME}_POSIX_SOURCES}
  ${${PROJECT_NAME}_SYNTAX_SOURCES}
  ${${PROJECT_NAME}_UTILITY_SOURCES}
  src/object.cpp
  src/evaluate.cpp
  src/openscenario_interpreter.cpp
  src/record.cpp
  src/scope.cpp)

target_link_libraries(${PROJECT_NAME} Boost::filesystem glog pugixml)

# workaround to allow deprecated header to build on both galactic and humble
if(${tf2_geometry_msgs_VERSION} VERSION_LESS 0.18.0)
  target_compile_definitions(${PROJECT_NAME} PUBLIC
    USE_TF2_GEOMETRY_MSGS_DEPRECATED_HEADER
  )
endif()

rclcpp_components_register_nodes(${PROJECT_NAME} "openscenario_interpreter::Interpreter")

# ------------------------------------------------------------------------------
#  openscenario_interpreter_node
# ------------------------------------------------------------------------------

ament_auto_add_executable(${PROJECT_NAME}_node src/${PROJECT_NAME}_node.cpp)
target_link_libraries(${PROJECT_NAME}_node ${PROJECT_NAME})

# ------------------------------------------------------------------------------
#  test
# ------------------------------------------------------------------------------

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()
  ament_add_gtest(test_syntax test/test_syntax.cpp)
  target_link_libraries(test_syntax ${PROJECT_NAME})
endif()

ament_auto_package()
