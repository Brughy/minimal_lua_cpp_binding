cmake_minimum_required (VERSION 3.16)

if(NOT DEFINED LUA_ENABLE_SHARED)
    set(LUA_ENABLE_SHARED 0)
endif ()

if(NOT DEFINED LUAVER)
    set(LUAVER 5.4.4)
endif ()

if(LUA_ENABLE_SHARED EQUAL 1)
	set(LIB_MODE "shared")
else()
	set(LIB_MODE "static")	
endif()	

if(NOT DEFINED NO_FETCH)
    set(NO_FETCH OFF)
endif ()

set(FETCHCONTENT_FULLY_DISCONNECTED ${NO_FETCH})

add_definitions(-DUSE_CMAKE)

set(PRJ_NAME simple_lua_cpp_binding)
project(${PRJ_NAME})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wno-format-truncation -DLUA_USE_DLOPEN")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DLUA_USE_DLOPEN")

#Fetch Dependencies
#include("${CMAKE_CURRENT_LIST_DIR}/external/googletest.cmake")

#Lua lib
set(DIR lua)
add_subdirectory(${DIR})
#set(LUA_LIB ${DIR}_lib_${LIB_MODE})
set(LUA_LIB ${DIR})

#Lua binding
#include("${CMAKE_CURRENT_LIST_DIR}/${DIR}")
set(DIR luabinding)
add_subdirectory(${DIR})
#set(LUABINDING_LIB ${DIR}_lib_${LIB_MODE})
set(LUABINDING_LIB ${DIR})

#Lua examples
set(DIR examples)
add_subdirectory(${DIR})
