# Not sure if it has to be this high
cmake_minimum_required(VERSION 3.21)

# Check if VCPKG is installed and usable for us.
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(vcpkg_file "$ENV{VCPKG_ROOT}\\scripts\\buildsystems\\vcpkg.cmake")
  cmake_path(SET vcpkg_file NORMALIZE "${vcpkg_file}")
  set(CMAKE_TOOLCHAIN_FILE "${vcpkg_file}" CACHE FILEPATH "vcpkg toolchain file")
endif()

project("carbon")

set(CMAKE_CXX_STANDARD 20)

# NVIDIA Aftermath as a CMake target
if (EXISTS "${CMAKE_SOURCE_DIR}/external/nv-aftermath" AND CMAKE_BUILD_TYPE STREQUAL "Debug" AND WIN32)
    set(NV_AFTERMATH_DIRECTORY "${CMAKE_SOURCE_DIR}/external/nv-aftermath")
    message(STATUS "Found NVIDIA Aftermath")
    
    add_library(nv-aftermath UNKNOWN IMPORTED GLOBAL)
    add_library(nv::aftermath ALIAS nv-aftermath)
    set_target_properties(nv-aftermath PROPERTIES IMPORTED_LOCATION "${NV_AFTERMATH_DIRECTORY}/lib/x64/GFSDK_Aftermath_Lib.x64.lib")
    set_property(TARGET nv-aftermath PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${NV_AFTERMATH_DIRECTORY}/include")
    option(WITH_NV_AFTERMATH "Use NVIDIA Aftermath" ON)

    # Move the Aftermath DLL into the binary folder.
    add_custom_target(nv-aftermath-copy ALL
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${NV_AFTERMATH_DIRECTORY}/lib/x64/GFSDK_Aftermath_Lib.x64.dll"
        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/")
endif()

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src")
