include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.12.1
)
FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)

add_executable(jimoTest
  ActionTests.cpp
  ActionHandlerTests.cpp
  DelegateTests.cpp
  EventArgsTests.cpp
  EventTests.cpp
  ObjectTests.cpp
  StopWatchTests.cpp
  StopWatchExceptionTests.cpp
  ThreadedActionHandlerTests.cpp
  TimerEventArgsTests.cpp
  TimerTests.cpp
  )

target_link_libraries(jimoTest
  PUBLIC
  GTest::GTest
  jimo)

add_test(jimoTests jimoTest)