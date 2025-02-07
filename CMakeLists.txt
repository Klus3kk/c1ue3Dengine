cmake_minimum_required(VERSION 3.16)
project(ClueEngine)

# Set C standard and C++ standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Detect OS
if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(PLATFORM_WINDOWS TRUE)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(PLATFORM_MACOS TRUE)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(PLATFORM_LINUX TRUE)
endif()

# Set output directories globally
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

# Setting applies to all configurations (Windows, Linux, MacOS)
if (CMAKE_CONFIGURATION_TYPES) # Multi-config generators (Visual Studio)
    foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
        string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG_UPPER)
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} ${CMAKE_SOURCE_DIR}/bin)
    endforeach()
else() # Single-config generators (Makefile, Ninja)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
endif()

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

# Link libraries based on OS
if (PLATFORM_WINDOWS)
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
elseif (PLATFORM_MACOS)
    find_library(COCOA_FRAMEWORK Cocoa)
    find_library(IOKIT_FRAMEWORK IOKit)
    find_library(COREVIDEO_FRAMEWORK CoreVideo)
    target_link_libraries(ClueEngine
        OpenGL::GL
        glfw
        GLEW
        assimp
        ${COCOA_FRAMEWORK}
        ${IOKIT_FRAMEWORK}
        ${COREVIDEO_FRAMEWORK}
    )
elseif (PLATFORM_LINUX)
    find_package(glfw3 REQUIRED)
    find_package(GLEW REQUIRED)
    find_package(assimp REQUIRED)
    target_link_libraries(ClueEngine
        OpenGL::GL
        glfw
        GLEW
        assimp
        pthread
        dl
    )
endif()

# Copy DLLs (Windows only)
if (PLATFORM_WINDOWS)
    add_custom_command(TARGET ClueEngine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/external ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    )
endif()

# Copy shaders and resources
add_custom_command(TARGET ClueEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/shaders ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders
)

add_custom_command(TARGET ClueEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/resources ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources
)

# Set working directory in Visual Studio
if (PLATFORM_WINDOWS)
    set_target_properties(ClueEngine PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
endif()

# Compiler settings
if (MSVC)
    target_compile_options(ClueEngine PRIVATE /W4)
else()
    target_compile_options(ClueEngine PRIVATE -Wall -Wextra -Wno-error)
endif()
