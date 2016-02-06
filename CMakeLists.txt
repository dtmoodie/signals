cmake_minimum_required(VERSION 2.8)

project(advanced_signals)

find_package(Boost REQUIRED thread system)
  include_directories(${Boost_INCLUDE_DIRS})
  link_directories(${Boost_LIBRARY_DIR_RELEASE})

find_package(OpenCV REQUIRED core)
  include_directories(${OpenCV_INCLUDE_DIRS})

file(GLOB_RECURSE src "signals/*.cpp")
file(GLOB_RECURSE hdr "signals/*.h" "signals/*.hpp")

add_library(signals SHARED ${src} ${hdr})
target_link_libraries(signals ${OpenCV_LIBS})

add_executable(test_signals main.cpp)
target_link_libraries(test_signals signals ${OpenCV_LIBS} ${Boost_LIBRARIES})

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
