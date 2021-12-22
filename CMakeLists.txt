message(STATUS "CMake version is ${CMAKE_VERSION}")
if(${CMAKE_VERSION} VERSION_LESS "3.18.1")
    message(STATUS "At least CMake 3.18.1 is required")
endif()

cmake_minimum_required (VERSION 3.18.1)
project (fleur CXX)

# Add source files.
# "GLOB_RECURSE" for subfolders, otherwise "GLOB".
file(GLOB SOURCES
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
)

# Determine configuration.
if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(FLEUR_CONFIG "debug")
elseif(${CMAKE_BUILD_TYPE} STREQUAL "Release")
    set(FLEUR_CONFIG "release")
elseif(${CMAKE_BUILD_TYPE} STREQUAL "RelWithDebInfo")
    set(FLEUR_CONFIG "release-with-debug-info")
elseif(${CMAKE_BUILD_TYPE} STREQUAL "MinSizeRel")
    set(FLEUR_CONFIG "min-size-release")
else()
    set(FLEUR_CONFIG "unsupported")
    message(FATAL_ERROR "Build configuration is unsupported")
endif()

# Determine architecture.
if(${CMAKE_SIZEOF_VOID_P} STREQUAL "8")
    set(FLEUR_ARCH "64")
elseif(${CMAKE_SIZEOF_VOID_P} STREQUAL "4")
    set(FLEUR_ARCH "32")
else()
    set(FLEUR_ARCH "unsupported")
    message(FATAL_ERROR "Architecture is unsupported")
endif()

# Determine operating system.
if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    set(FLEUR_OS "win")
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set(FLEUR_OS "linux")
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    set(FLEUR_OS "osx")
else()
    set(FLEUR_OS ${CMAKE_SYSTEM_NAME})
    message(WARNING "Unexpected operating system name")
endif()

add_executable(${PROJECT_NAME} ${SOURCES})

# Add header files.
target_include_directories(${PROJECT_NAME}
    PRIVATE ${CMAKE_SOURCE_DIR}/src
    PRIVATE ${CMAKE_SOURCE_DIR}/third-party/include
)

set(FLEUR_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/bin/${FLEUR_OS}-x${FLEUR_ARCH}/${FLEUR_CONFIG})

# Specify output directory, executable name and C++ standard.
set_target_properties(${PROJECT_NAME} PROPERTIES
    # "$<0:>" is a generator expression, it prevents multi-configuration generators
    # from appending a per-configuration sub-directory to the specified path.
    RUNTIME_OUTPUT_DIRECTORY ${FLEUR_OUTPUT_DIR}/$<0:>
    OUTPUT_NAME "fleurc"
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

# Set warning level.
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        /WX     # Treat warnings as errors.
        /W4     # Warning level.
        /wd4458 # Suppress warning "declaration of 'x' hides class member".
    )
else()
    target_compile_options(${PROJECT_NAME} PRIVATE
        -pedantic     # No extensions.
        -Wall -Wextra # Warnings.
        -Werror       # Warnings are errors.
    )
endif()

# Use multiple processes to build faster with Visual Studio.
if(CMAKE_GENERATOR MATCHES "Visual Studio")
    target_compile_options(${PROJECT_NAME} PRIVATE /MP)
endif()

# Define "FLEUR_DEBUG" only in Debug mode.
if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE FLEUR_DEBUG)
endif()

set(FLEUR_LIB_DIR ${CMAKE_SOURCE_DIR}/third-party/lib)

find_library(FLEUR_LLVM_LIB
    NAMES LLVM-C
    PATHS ${FLEUR_LIB_DIR}
    PATH_SUFFIXES lib64 lib so a
)

#message(STATUS "Sources are ${SOURCES}")
#message(STATUS "Libraries are ${FLEUR_LLVM_LIB}")

target_link_libraries(${PROJECT_NAME} ${FLEUR_LLVM_LIB})

#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

#if(MSVC)
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z")
#    message(STATUS "Microsoft Visual Studio is used")
#endif(MSVC)

#target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
