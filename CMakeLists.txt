cmake_minimum_required(VERSION 3.10)
project(MNIST LANGUAGES CXX CUDA)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# search CUDA
find_package(CUDA REQUIRED)
include_directories(${CUDA_INCLUDE_DIRS})
set(CUDA_LIBRARIES "${CUDA_TOOLKIT_ROOT_DIR}/lib64/libcudart.so")
 

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
find_package(TensorRT REQUIRED)

add_executable(runtime main.cpp)
target_link_libraries(runtime PUBLIC ${TensorRT_LIBRARIES} ${CUDA_LIBRARIES})
target_include_directories(runtime PUBLIC ${TensorRT_INCLUDE_DIRS} ${CUDA_INCLUDE_DIRS})

add_executable(convert convert.cpp)
target_link_libraries(convert PUBLIC ${TensorRT_LIBRARIES} ${CUDA_LIBRARIES})
target_include_directories(convert PUBLIC ${TensorRT_INCLUDE_DIRS} ${CUDA_INCLUDE_DIRS})
