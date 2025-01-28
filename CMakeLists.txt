cmake_minimum_required(VERSION 3.16)
project(ClueEngine)

# Set C standard and C++ standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Ensure the executable goes to bin/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/external
)

# Source files
file(GLOB_RECURSE SOURCES src/*.c src/*.cpp)
file(GLOB_RECURSE HEADERS include/*.h include/*.hpp)

# Add the executable
add_executable(ClueEngine ${SOURCES} ${HEADERS})
add_compile_options(/W0)

# Link libraries
target_link_libraries(ClueEngine
    OpenGL::GL
    ${CMAKE_SOURCE_DIR}/lib/glfw3.lib
    ${CMAKE_SOURCE_DIR}/lib/glew32s.lib
    ${CMAKE_SOURCE_DIR}/lib/assimp-vc143-mtd.lib
    ${CMAKE_SOURCE_DIR}/lib/ode_doubled.lib
    ${CMAKE_SOURCE_DIR}/lib/soil2.lib
    ${CMAKE_SOURCE_DIR}/lib/cjson.lib
    ${CMAKE_SOURCE_DIR}/lib/tinyfiledialogs32.lib
    ${CMAKE_SOURCE_DIR}/lib/tinyfiledialogs64.lib
)

# ✅ Copy shaders/, resources/, and external DLLs to bin/
add_custom_command(TARGET ClueEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/external ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}

    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/shaders ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders

    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/resources ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources
)

# Set compiler options
if (MSVC)
    target_compile_options(ClueEngine PRIVATE /W4)
else()
    target_compile_options(ClueEngine PRIVATE -Wall -Wextra -Wno-error)
endif()
