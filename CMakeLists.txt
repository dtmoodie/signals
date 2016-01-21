cmake_minimum_required(VERSION 2.8)

project(advanced_signals)

find_package(Boost REQUIRED thread)
  include_directories(${Boost_INCLUDE_DIRS})
  link_directories(${Boost_LIBRARY_DIR_RELEASE})

find_package(OpenCV REQUIRED core)
  include_directories(${OpenCV_INCLUDE_DIRS})
  
add_executable(main main.cpp)
target_link_libraries(main ${OpenCV_LIBS})