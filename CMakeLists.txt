cmake_minimum_required(VERSION 2.8)
set(CMAKE_DEBUG_POSTFIX "d")
project(advanced_signals)

find_package(Boost REQUIRED system log log_setup thread unit_test_framework)
  include_directories(${Boost_INCLUDE_DIRS})
  link_directories(${Boost_LIBRARY_DIR_RELEASE})
  link_directories(${Boost_LIBRARY_DIR})


find_package(PythonLibs QUIET)
if(PythonLibs_FOUND)
  add_definitions(-DHAVE_BOOST_PYTHON)
  include_directories(${PythonLibs_INCLUDE_DIRS})
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB_RECURSE src "signals/*.cpp")
file(GLOB_RECURSE hdr "signals/*.h" "signals/*.hpp")

add_library(signals SHARED ${src} ${hdr})
target_link_libraries(signals ${OpenCV_LIBS} ${Boost_LIBRARIES})

add_executable(signal_example main.cpp)
target_link_libraries(signal_example signals ${Boost_LIBRARIES})
set_target_properties(signal_example PROPERTIES FOLDER examples)
if(MSVC)
  set_target_properties(signal_example PROPERTIES COMPILE_FLAGS "/bigobj")
endif()


CONFIGURE_FILE(SignalsConfig.cmake.in ${CMAKE_BINARY_DIR}/SignalsConfig.cmake @ONLY)

set(Signals_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR} CACHE PATH "")

install(TARGETS signals
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)

install(DIRECTORY /
  DESTINATION include
  FILES_MATCHING PATTERN "*.h")

install(DIRECTORY /
  DESTINATION include
  FILES_MATCHING PATTERN "*.hpp")

install(FILES ${CMAKE_BINARY_DIR}/SignalsConfig.cmake
  DESTINATION ./)

		
file(GLOB_RECURSE test_srcs "tests/*.cpp")

add_executable(test_signals ${test_srcs})
target_link_libraries(test_signals signals)
set_target_properties(test_signals PROPERTIES FOLDER Tests)
