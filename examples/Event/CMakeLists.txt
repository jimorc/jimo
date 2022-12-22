cmake_minimum_required(VERSION 3.22)

add_executable(Event 
  Event.cpp)

target_link_libraries(Event
  PRIVATE
  jimo)

target_compile_features(Event INTERFACE cxx_std_20)