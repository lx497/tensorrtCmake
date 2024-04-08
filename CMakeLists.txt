cmake_minimum_required(VERSION 3.10)
project(MNIST LANGUAGES CXX CUDA)


# 寻找CUDA
find_package(CUDA REQUIRED)
include_directories(${CUDA_INCLUDE_DIRS})
set(CUDA_LIBRARIES "${CUDA_TOOLKIT_ROOT_DIR}/lib64/libcudart.so")
 

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
find_package(TensorRT REQUIRED)

add_executable(runtime main.cpp)
target_link_libraries(runtime PUBLIC ${TensorRT_LIBRARIES} ${CUDA_LIBRARIES})
target_include_directories(runtime PUBLIC ${TensorRT_INCLUDE_DIRS} ${CUDA_INCLUDE_DIRS})
