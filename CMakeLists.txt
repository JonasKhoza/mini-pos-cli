cmake_minimum_required(VERSION 3.16)#Declares the minimum CMake version required and enables behavior/policy defaults introduced up to that version.
set(CMAKE_POLICY_DEFAULT_CMP0135 NEW)
project(book-cli #Sets PROJECT_NAME
    VERSION 0.1.0 #Sets PROJECT_VERSION
    DESCRIPTION "Small inventory & sales CLI with JSON persistence and optional HTTP API"
    LANGUAGES CXX #enables language-specific checks and compiler probing for C++
)

set(CMAKE_CXX_STANDARD 17)#Suggests a default C++ standard for targets (global variable).
set(CMAKE_CXX_STANDARD_REQUIRED ON) #Make CMAKE_CXX_STANDARD a required standard rather than a hint.If the compiler doesn’t support it, configure will error.
set(CMAKE_CXX_EXTENSIONS OFF)#Disables compiler-specific language extensions (e.g., gnu++17) and prefers standard-conforming mode.

option(BUILD_TESTS "Build unit tests" OFF) #Declares a user-configurable boolean option BUILD_TESTS (defaults to ON).Users/CI can override with -DBUILD_TESTS=OFF.
option(BUILD_EXAMPLES "Build examples" OFF)#Another toggle for examples.

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)#Sets where static library files (.a, .lib) will be placed after build (in the build tree).
#${CMAKE_BINARY_DIR} is the top-level build directory (i.e., build/ when you use -B build).
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib) #set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)#Sets where executables go.

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)#Requests compile_commands.json generation in the build directory. Useful for language servers (clangd), code navigation, and external tools.

set(PROJECT_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include) #Expose the include path from root so subdirs reuse it
add_subdirectory(src)#Tells CMake to chdir into src/ and process src/CMakeLists.txt.
#Delegates module-level target declarations to the src directory. Variables set in root flow down; targets declared in subdirs are added to the global build graph.

if(BUILD_TESTS) #Conditional: if tests are enabled, enable_testing() then add_subdirectory(tests).
  enable_testing() #enable_testing() effect: Registers CTest support (so ctest can discover tests).
  add_subdirectory(tests) #add_subdirectory(tests) effect: Loads tests/CMakeLists.txt to create test targets.
endif()

install(TARGETS book-cli #Declares runtime install rules for the book-cli executable (puts it into bin/ on cmake --install or make install).
  RUNTIME DESTINATION bin #Why: Makes packaging and system install easier for users or for packaging with CPack.
)

include(CPack)#What: Loads the CPack module to enable packaging.
#After configuring variables (CPACK_*), running cpack produces installers (zip, tar, platform-specific packages depending on configuration)

set(CPACK_PACKAGE_NAME "book-cli") #Provides metadata for CPack.
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})#Provides metadata for CPack.