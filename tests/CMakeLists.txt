#enable_testing()

#include_directories(${CMAKE_CURRENT_BINARY_DIR}/inst_shapef.dir/Debug)
add_executable(inst_shapef inst_shapef.cpp)

add_test(NAME inst_test
  WORKING_DIRECTORY $<TARGET_FILE_DIR:MapWinGIS>
  COMMAND $<TARGET_FILE_DIR:inst_shapef>)
