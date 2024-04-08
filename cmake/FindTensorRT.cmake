# Copyright 2023 Aron Rubin
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the “Software”), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so.

# THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
# Locate the system installed TensorRT
#
# The following variables will be set:
#   TensorRT_FOUND        - Set to true if TensorRT can be found
#   TensorRT_INCLUDE_DIRS - The path to the TensorRT header files for all components
#   TensorRT_LIBRARIES    - A list of TensorRT library targets for all components
#   TensorRT_EXECUTABLE   - The full path of trtexec
#
# Available components:
#   nvinfer (default), nvinfer_plugin, nvparsers, nvonnxparsers
#


set(TENSORRT_DIR /home/lx497/TensorRT-10.0.0.6/)
set(TensorRT_INCLUDE_DIRS /home/lx497/TensorRT-10.0.0.6/include/)
set(TensorRT_LIBRARIES /home/lx497/TensorRT-10.0.0.6/targets/x86_64-linux-gnu/lib/)
set(TensorRT_EXECUTABLE /home/lx497/TensorRT-10.0.0.6/targets/x86_64-linux-gnu/bin/trtexec)

macro(tensorrt_find_component _component _header _library)
  message(STATUS "Searching for header ${_header}")
  find_path( ${_component}_INCLUDE_DIR ${_header}
    HINTS
      ${TENSORRT_DIR}
      ${CUDA_TOOLKIT_ROOT_DIR}
      ${CUDAToolkit_ROOT_DIR}
    PATH_SUFFIXES
      include)
  message("TENSORRT_DIR:${TENSORRT_DIR}")
  if(${_component}_INCLUDE_DIR)
    message(STATUS "Found TensorRT header ${_header} in ${${_component}_INCLUDE_DIR}")
  else()
    message(FATAL_ERROR "TensorRT ${_component} header ${_header} not found in paths "
      "${TENSORRT_DIR} ${CUDA_TOOLKIT_ROOT_DIR} ${CUDAToolkit_ROOT_DIR}. Set TENSORRT_ROOT")
  endif ()

  message(STATUS "Searching for library ${_library}")
  find_library( ${_component}_LIBRARY
    NAMES
      ${_library}
      lib${_library}
    HINTS
      ${TENSORRT_DIR}
      ${CUDA_TOOLKIT_ROOT_DIR}
      ${CUDAToolkit_ROOT_DIR}
    PATH_SUFFIXES
      lib
      lib64
      lib/x64)
  if(${_component}_LIBRARY)
    message(STATUS "Found TensorRT ${_library} for ${_component} at ${${_component}_LIBRARY}")
  else()
    message(FATAL_ERROR "TensorRT ${_component} lib ${_library} not found in paths "
        "${TENSORRT_DIR} ${CUDA_TOOLKIT_ROOT_DIR} ${CUDAToolkit_ROOT_DIR}. Set TENSORRT_ROOT")
  endif()

  if(${_component}_INCLUDE_DIR AND ${_component}_LIBRARY)
    set(TensorRT_${_component}_FOUND TRUE)
    add_library(TensorRT::${_component} SHARED IMPORTED)
    set_property(TARGET TensorRT::${_component} APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
    set_property(TARGET TensorRT::${_component} PROPERTY IMPORTED_LOCATION_RELEASE "${${_component}_LIBRARY}")
    set_property(TARGET TensorRT::${_component} PROPERTY IMPORTED_IMPLIB_RELEASE "${${_component}_LIBRARY}")
    target_include_directories(TensorRT::${_component} INTERFACE ${${_component}_INCLUDE_DIR})
  endif()
  mark_as_advanced(
    ${_component}_INCLUDE_DIR
    ${_component}_LIBRARY
    TensorRT_${_component}_FOUND )

  list(APPEND TensorRT_INCLUDE_DIRS ${${_component}_INCLUDE_DIR})
  message(STATUS "TensorRT_INCLUDE_DIRS set to ${TensorRT_INCLUDE_DIRS}")
endmacro()

if (NOT TensorRT_FIND_COMPONENTS)
  set(TensorRT_FIND_COMPONENTS nvinfer nvinfer_plugin)
endif ()



message("TensorRT_EXECUTABLE ${TensorRT_LIBRARIES}")

if (NOT DEFINED TensorRT_ROOT)
  if (NOT ENV{TensorRT_ROOT})
    set(TensorRT_DIR /usr)
  else()
    set(TensorRT_DIR $ENV{TensorRT_DIR})
  endif()
endif ()

if (FaceMeSDK_DIR)
  message(STATUS "TensorRT_DIR process")
  if (TensorRT_FIND_VERSION)
    find_package( TensorRT ${TensorRT_FIND_VERSION} NO_MODULE)
  else()
    find_package( TensorRT NO_MODULE)
  endif()
elseif (NOT TensorRT_FOUND)
  message(STATUS "Finding TensorRT process ${TensorRT_FIND_COMPONENTS}")
  foreach(cmpt ${TensorRT_FIND_COMPONENTS})
    # handle 'core' alias
    if(cmpt STREQUAL "core" OR cmpt STREQUAL "TensorRT")
      set(cmpt "nvinfer")
    endif()
    message(STATUS "Finding TensorRT component: '${cmpt}'")
    if (cmpt STREQUAL "nvinfer")
      tensorrt_find_component(${cmpt} NvInfer.h nvinfer)

      if(TensorRT_${cmpt}_FOUND)
        add_library(TensorRT ALIAS TensorRT::nvinfer)

        file(READ "${nvinfer_INCLUDE_DIR}/NvInferVersion.h" nvinfer_version_header)
        string(REGEX REPLACE ".*#[ \t]*define[ \t]*NV_TENSORRT_MAJOR[ \t]*([0-9]+).*" "\\1" NV_TENSORRT_MAJOR "${nvinfer_version_header}")
        string(REGEX REPLACE ".*#[ \t]*define[ \t]*NV_TENSORRT_MINOR[ \t]*([0-9]+).*" "\\1" NV_TENSORRT_MINOR "${nvinfer_version_header}")
        string(REGEX REPLACE ".*#[ \t]*define[ \t]*NV_TENSORRT_PATCH[ \t]*([0-9]+).*" "\\1" NV_TENSORRT_PATCH "${nvinfer_version_header}")
        set(TensorRT_VERSION ${NV_TENSORRT_MAJOR}.${NV_TENSORRT_MINOR}.${NV_TENSORRT_PATCH})
        message(STATUS "Found TensorRT version: ${TensorRT_VERSION}")

        find_program(
          TensorRT_EXECUTABLE trtexec
          HINTS
            ${TENSORRT_DIR}
            ${CUDA_TOOLKIT_ROOT_DIR}
            ${CUDAToolkit_ROOT_DIR}
            /usr/src/tensorrt
          PATH_SUFFIXES
            bin )
        message(STATUS "Found TensorRT executable: ${TensorRT_EXECUTABLE}")
      endif()
    elseif( cmpt STREQUAL "nvinfer_plugin")
      tensorrt_find_component(${cmpt} NvInferPlugin.h nvinfer_plugin)
    elseif( cmpt STREQUAL "nvparsers")
      tensorrt_find_component(${cmpt} NvCaffeParser.h nvparsers)
    elseif( cmpt STREQUAL "nvonnxparsers")
      tensorrt_find_component(${cmpt} NvOnnxParser.h nvonnxparser)
    endif()
    if(TensorRT_${cmpt}_FOUND)
      list(APPEND TensorRT_LIBRARIES TensorRT::${cmpt})
    endif()
  endforeach()
  list(REMOVE_DUPLICATES TensorRT_INCLUDE_DIRS)
  list(REMOVE_DUPLICATES TensorRT_LIBRARIES)

  message(STATUS "TensorRT include paths: ${TensorRT_INCLUDE_DIRS}")
  message(STATUS "TensorRT libraries: ${TensorRT_LIBRARIES}")
  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(
    TensorRT
    REQUIRED_VARS TensorRT_INCLUDE_DIRS
    VERSION_VAR TensorRT_VERSION
    HANDLE_COMPONENTS )
endif()