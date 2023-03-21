project(query_wod_memic_cap_lib)
cmake_minimum_required(VERSION 2.8)

# add_executable(${PROJECT_NAME} main.c get_cap.h get_cap.c)

# Link to system installed library (in /usr/infiniband/)
#   ==> ibverbs 
#   ==> mlx5
#target_link_libraries(${PROJECT_NAME} ibverbs)
#target_link_libraries(${PROJECT_NAME} mlx5)

add_library(${PROJECT_NAME} src/query_wod_memic_cap.c)
target_link_libraries(${PROJECT_NAME} ibverbs)
target_link_libraries(${PROJECT_NAME} mlx5)