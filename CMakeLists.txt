cmake_minimum_required(VERSION 3.20)

project(ASSET_PAC)
set(CMAKE_CXX_STANDARD 20)

get_filename_component(INCLUDE_DIR "include" ABSOLUTE)
get_filename_component(THIRD_PARTY "third_party" ABSOLUTE)

add_subdirectory(src/Packer)
#add_subdirectory(src/Viewer)

set_target_properties(AssetPac PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

