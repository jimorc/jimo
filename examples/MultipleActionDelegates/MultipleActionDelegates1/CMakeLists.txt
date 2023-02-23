cmake_minimum_required(VERSION 3.22)

add_executable(MultipleActionDelegates1 
MultipleActionDelegates1.cpp)

target_link_libraries(MultipleActionDelegates1
  PRIVATE
  jimo)

target_compile_features(MultipleActionDelegates1 INTERFACE cxx_std_20)