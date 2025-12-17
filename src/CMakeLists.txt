cmake_minimum_required(VERSION 3.16) # optional but safe in subdir
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

include(FetchContent) #Loads CMake's FetchContent module so you can declare and fetch external projects at configure time.

# FetchContent_Declare( #Declares the nlohmann/json dependency and downloads/configures it at configure time.
#   nlohmann_json
#   GIT_REPOSITORY https://github.com/nlohmann/json.git
#   GIT_TAG v3.11.2
# )

FetchContent_Declare(#lighter and faster compared to the top one 
  nlohmann_json
  URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(nlohmann_json) #either makes an already-present copy available or fetches it then adds its targets to your build.

add_library(helpers_lib 
helpers/GlobalHelpers.cpp
helpers/JsonStore.cpp
)
target_link_libraries(helpers_lib
  PUBLIC nlohmann_json::nlohmann_json
)

add_library(domain_lib STATIC
 # domain/Product.cpp
  domain/inventory.cpp
  domain/TransactionManager.cpp
)
# give domain_lib access to the public headers
target_include_directories(domain_lib
  PUBLIC ${PROJECT_INCLUDE_DIR}
)
target_compile_features(domain_lib PUBLIC cxx_std_17)
target_link_libraries(domain_lib
  PUBLIC nlohmann_json::nlohmann_json
)

# target_compile_features(domain PUBLIC cxx_std_17)

# add_library(util STATIC
#   util/atomic_file_write.cpp
# )
# target_include_directories(util PUBLIC ${PROJECT_INCLUDE_DIR})
# target_compile_features(util PUBLIC cxx_std_17)

# add_library(persistence STATIC
#   persistence/JsonStore.cpp
# )
# target_include_directories(persistence PUBLIC ${PROJECT_INCLUDE_DIR})
# target_link_libraries(persistence PRIVATE nlohmann_json::nlohmann_json util)
# target_compile_features(persistence PUBLIC cxx_std_17)

add_executable(book-cli
  ${SRC_DIR}/main.cpp
  ${SRC_DIR}/app/app.cpp   #another .cpp file that needs to be compiled and linked to our book-cli executable
  #${SRC_DIR}/cli/CLI11.hpp
  ${SRC_DIR}/helpers/GlobalHelpers.cpp
  ${SRC_DIR}/helpers/JsonStore.cpp
)

target_include_directories(book-cli PRIVATE ${PROJECT_INCLUDE_DIR})#command that specifies which directories the compiler should search for header files when building the specific target named book-cli
target_include_directories(helpers_lib PUBLIC ${PROJECT_INCLUDE_DIR})
target_link_libraries(book-cli PRIVATE 
domain_lib
#persistence util
)
target_link_libraries(book-cli PRIVATE nlohmann_json::nlohmann_json)
target_compile_features(book-cli PRIVATE cxx_std_17)
target_compile_features(helpers_lib PUBLIC cxx_std_17)

#add_library(book-cli::app ALIAS book-cli)