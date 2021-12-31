cmake_minimum_required (VERSION 3.16)
project (test CXX)

# Add source files.
# "GLOB_RECURSE" for subfolders, otherwise "GLOB".
file(GLOB_RECURSE SOURCES
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/*.c"
    "${CMAKE_SOURCE_DIR}/../../src/*.cpp"
    "${CMAKE_SOURCE_DIR}/../../src/*.c"
)

# Exclude Fleur "main.cpp" file.
# The regex excludes every path ending with the given file name.
# list(FILTER SOURCES EXCLUDE REGEX ".*main\\.cpp$")

# Exclude Fleur "main.cpp" file.
list(REMOVE_ITEM SOURCES "${CMAKE_SOURCE_DIR}/../../src/main.cpp")
#message(${SOURCES})

# Determine configuration.
if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(TEST_CONFIG "debug")
elseif(${CMAKE_BUILD_TYPE} STREQUAL "Release")
    set(TEST_CONFIG "release")
elseif(${CMAKE_BUILD_TYPE} STREQUAL "RelWithDebInfo")
    set(TEST_CONFIG "release-with-debug-info")
elseif(${CMAKE_BUILD_TYPE} STREQUAL "MinSizeRel")
    set(TEST_CONFIG "min-size-release")
else()
    set(TEST_CONFIG "unknown-config")
    message(FATAL_ERROR "Build configuration is unknown")
endif()

# Determine architecture.
if(${CMAKE_SIZEOF_VOID_P} STREQUAL "8")
    set(TEST_ARCH "64")
elseif(${CMAKE_SIZEOF_VOID_P} STREQUAL "4")
    set(TEST_ARCH "32")
else()
    set(TEST_ARCH "unknown-arch")
    message(FATAL_ERROR "Architecture is unknown")
endif()

# Determine operating system.
if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    set(TEST_OS "win")
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set(TEST_OS "linux")
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    set(TEST_OS "osx")
else()
    set(TEST_OS "unknown-os")
    message(WARNING "Operating system name is unknown")
endif()

add_executable(${PROJECT_NAME} ${SOURCES})

# Add header files.
target_include_directories(${PROJECT_NAME}
    PRIVATE ${CMAKE_SOURCE_DIR}/src
    PRIVATE ${CMAKE_SOURCE_DIR}/../../src
)

set(TEST_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/bin/${TEST_OS}-x${TEST_ARCH}/${TEST_CONFIG})

# Specify output directory, executable name and C++ standard.
set_target_properties(${PROJECT_NAME} PROPERTIES
    # "$<0:>" is a generator expression, it prevents multi-configuration generators
    # from appending a per-configuration sub-directory to the specified path.
    RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR}/$<0:>
    OUTPUT_NAME "test"
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

# Set warning level.
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        /WX                 # Treat warnings as errors.
        /W4                 # Warning level.
        /wd4458             # Suppress warning "declaration of 'x' hides class member".
        /wd4996             # Suppress warning "'fopen': This function or variable may be unsafe. Consider using fopen_s instead."
        #/fsanitize=address
    )
else()
    target_compile_options(${PROJECT_NAME} PRIVATE
        -pedantic           # No extensions.
        -Wall -Wextra       # Warnings.
        -Werror             # Warnings are errors.
        #-fsanitize=address
    )
endif()

# Use multiple processes to build faster with Visual Studio.
if(CMAKE_GENERATOR MATCHES "Visual Studio")
    target_compile_options(${PROJECT_NAME} PRIVATE /MP)
endif()
