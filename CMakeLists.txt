# We require CMake >= 3.2
cmake_minimum_required(VERSION 3.2)

# add this options before PROJECT keyword
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

# CMake policies (can not be handled elsewhere)
cmake_policy(SET CMP0005 OLD)
if(POLICY CMP0043)
  cmake_policy(SET CMP0043 OLD) # Disable 'Ignore COMPILE_DEFINITIONS_<Config> properties'
endif()

if(POLICY CMP0054)
  cmake_policy(SET CMP0054 NEW) # Only interpret if() arguments as variables or keywords when unquoted - prevents intepreting if (SOME_STRING_VARIABLE MATCHES "MSVC") as if (SOME_STRING_VARIABLE MATCHES "1")
endif()

# Set projectname (must be done AFTER setting configurationtypes)
project(WARpp)

# Set RPATH-handing (CMake parameters)
set(CMAKE_SKIP_BUILD_RPATH 0)
set(CMAKE_BUILD_WITH_INSTALL_RPATH 0)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH 1)

# set macro-directory
list(APPEND CMAKE_MODULE_PATH
  "${CMAKE_SOURCE_DIR}/cmake/macros"
  "${CMAKE_SOURCE_DIR}/dep/cotire/CMake")

# build in Release-mode by default if not explicitly set
if( NOT CMAKE_BUILD_TYPE )
  set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()

include(CheckCXXSourceRuns)
include(CheckIncludeFiles)

# set default buildoptions and print them
include(cmake/options.cmake)

# turn off PCH totally if enabled (hidden setting, mainly for devs)
if( NOPCH )
  set(USE_COREPCH 0)
  set(USE_SCRIPTPCH 0)
endif()

include(ConfigureBaseTargets)
include(CheckPlatform)

include(GroupSources)
include(AutoCollect)

find_package(PCHSupport)
find_package(MySQL)

# print out the results before continuing
include(cmake/showoptions.cmake)

# add dependencies
add_subdirectory(dep)

# add core sources
add_subdirectory(src)
