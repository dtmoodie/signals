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

find_package(Boost QUIET COMPONENTS unit_test_framework)
if(Boost_UNIT_TEST_FRAMEWORK_FOUND)		
  file(GLOB_RECURSE test_srcs "tests/*.cpp")
  foreach(testSrc ${test_srcs})
    get_filename_component(testName ${testSrc} NAME_WE)
    add_executable(${testName} ${testSrc})
    target_link_libraries(${testName} signals ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
    set_target_properties(${testName} PROPERTIES FOLDER Tests)
    set_target_properties(${testName} PROPERTIES FOLDER Tests/signals)
    set_target_properties(${testName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/tests)
	add_test(NANE ${testName}
	         WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin 
             COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/testBin/${testName})
  endforeach()
endif()