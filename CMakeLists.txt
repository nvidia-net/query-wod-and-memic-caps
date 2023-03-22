project(query_wod_memic_cap)
cmake_minimum_required(VERSION 2.8)

add_library(${PROJECT_NAME} src/query_wod_memic_cap.c)
message(${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} ibverbs)
target_link_libraries(${PROJECT_NAME} mlx5)

add_executable(${PROJECT_NAME}_test test/main.c)
target_include_directories(${PROJECT_NAME}_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME}_test ${PROJECT_NAME})