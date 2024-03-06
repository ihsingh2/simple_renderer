cmake_minimum_required(VERSION 3.18)
set(CMAKE_BUILD_TYPE Release)

project(cs7302
  VERSION 1.0 
  LANGUAGES C CXX
)

if (APPLE)
  set(CMAKE_MACOSX_RPATH ON)
endif()

if (MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D_CRT_SECURE_NO_WARNINGS")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
endif()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libraries")
set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

set(SOURCES
  "${SRC_DIR}/callback.cpp"
  "${SRC_DIR}/init.cpp"
  "${SRC_DIR}/render.cpp"
  "${SRC_DIR}/shapes.cpp"
)

set(EXEC_NAME "render")
add_executable(${EXEC_NAME} ${SOURCES})
include_directories(${INCLUDE_DIR})

set(GLFW_DIR "${LIB_DIR}/glfw")
set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")
add_subdirectory("${GLFW_DIR}")
target_link_libraries(${EXEC_NAME} "glfw" "${GLFW_LIBRARIES}")
target_include_directories(${EXEC_NAME} PRIVATE "${GLFW_DIR}/include")
target_compile_definitions(${EXEC_NAME} PRIVATE "GLFW_INCLUDE_NONE")

set(GLAD_DIR "${LIB_DIR}/glad")
add_library("glad" "${GLAD_DIR}/src/glad.c")
target_include_directories("glad" PRIVATE "${GLAD_DIR}/include")
target_include_directories(${EXEC_NAME} PRIVATE "${GLAD_DIR}/include")
target_link_libraries(${EXEC_NAME} "glad" "${CMAKE_DL_LIBS}")

set(GLM_INCLUDE_DIRS "${LIB_DIR}/glm")
include_directories(${GLM_INCLUDE_DIRS})
