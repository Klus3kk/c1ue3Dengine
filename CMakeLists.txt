cmake_minimum_required(VERSION 3.16)
project(ClueEngine)

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

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

# Set multi-config directories (for Visual Studio)
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

# Fetch External Dependencies
include(FetchContent)

# Fetch and build cJSON
FetchContent_Declare(
  cjson
  GIT_REPOSITORY https://github.com/DaveGamble/cJSON.git
  GIT_TAG        master
)
FetchContent_GetProperties(cjson)
if(NOT cjson_POPULATED)
    FetchContent_Populate(cjson)
    add_subdirectory(${cjson_SOURCE_DIR} ${cjson_BINARY_DIR})
endif()
include_directories(${cjson_SOURCE_DIR})

# Fetch and build tinyfiledialogs
FetchContent_Declare(
  tinyfiledialogs
  GIT_REPOSITORY https://github.com/native-toolkit/tinyfiledialogs.git
  GIT_TAG        master
)
FetchContent_GetProperties(tinyfiledialogs)
if(NOT tinyfiledialogs_POPULATED)
    FetchContent_Populate(tinyfiledialogs)
    add_library(tinyfiledialogs STATIC ${tinyfiledialogs_SOURCE_DIR}/tinyfiledialogs.c)
endif()
include_directories(${tinyfiledialogs_SOURCE_DIR})

# Assimp (3D Model Loader)
FetchContent_Declare(
  assimp
  GIT_REPOSITORY https://github.com/assimp/assimp.git
  GIT_TAG        master
)
FetchContent_MakeAvailable(assimp)

# SOIL2 (Image Loader)
FetchContent_Declare(
  soil2
  GIT_REPOSITORY https://github.com/SpartanJ/soil2.git
  GIT_TAG        master
)
FetchContent_MakeAvailable(soil2)
include_directories(${soil2_SOURCE_DIR}/src)

# Find GLFW (Windows / Linux / MacOS)
if (PLATFORM_LINUX OR PLATFORM_MACOS)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GLFW QUIET glfw3)
    if (NOT GLFW_FOUND)
        find_package(glfw3 CONFIG QUIET)
    endif()
    if (NOT GLFW_FOUND)
        find_path(GLFW_INCLUDE_DIR NAMES GLFW/glfw3.h PATHS /usr/include /usr/local/include)
        find_library(GLFW_LIBRARY NAMES glfw glfw3 PATHS /usr/lib /usr/local/lib)
        if (GLFW_INCLUDE_DIR AND GLFW_LIBRARY)
            set(GLFW_FOUND TRUE)
            set(GLFW_LIBRARIES ${GLFW_LIBRARY})
            include_directories(${GLFW_INCLUDE_DIR})
        endif()
    endif()
    if (NOT GLFW_FOUND)
        message(FATAL_ERROR "GLFW3 not found. Install it with your package manager.")
    endif()
endif()

# Find GLEW (Separate for Windows & Linux/macOS)
if (PLATFORM_WINDOWS)
    include_directories(${CMAKE_SOURCE_DIR}/include) # Windows uses prebuilt GLEW
else()
    find_package(GLEW REQUIRED)
    include_directories(${GLEW_INCLUDE_DIRS})
endif()

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/external
)

# Source files
file(GLOB_RECURSE SOURCES src/*.c src/*.cpp)
file(GLOB_RECURSE HEADERS include/*.h include/*.hpp)

# Ensure `add_executable` is declared BEFORE linking libraries
add_executable(ClueEngine ${SOURCES} ${HEADERS})

# Properly link libraries for **each platform**
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
        ${GLFW_LIBRARIES}
        ${GLEW_LIBRARY}
        assimp
        soil2
        cjson
        tinyfiledialogs
        m   
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
