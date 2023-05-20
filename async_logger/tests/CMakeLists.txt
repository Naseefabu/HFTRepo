enable_testing()
add_executable(WriteLogTest writelogtest.cpp)
add_test(writelogtest WriteLogTest)
target_include_directories(WriteLogTest PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_executable(NewFileMidnightCross NewFileMidnightTest.cpp)
add_test(newfilemidnightcrossed NewFileMidnightCross)
target_include_directories(NewFileMidnightCross PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_executable(StopNow StopImmediatelyTest.cpp)
add_test(stopnow StopNow)
target_include_directories(StopNow PUBLIC ${PROJECT_SOURCE_DIR}/include)