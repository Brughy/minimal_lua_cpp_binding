set(APP luabinding)
project(${APP})

if(NOT DEFINED LUA_ENABLE_SHARED)
    set(LUA_ENABLE_SHARED 0)
endif ()

set(LUA_LIB_SRCS 
    "${CMAKE_CURRENT_LIST_DIR}/luabinding.cpp"
)

set(LUA_LIB_INCLUDE 
    "${CMAKE_CURRENT_LIST_DIR}/luabinding.h"
)

set(INCLUDE ${LUA_LIB_INCLUDE})
set(SOURCE  ${LUA_LIB_SRCS})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# 1. lua library
if(LUA_ENABLE_SHARED EQUAL 1)
	set(LIB_MODE "shared")
	add_library(${APP}_lib_${LIB_MODE} SHARED ${SOURCE})
else()
	set(LIB_MODE "static")
	add_library(${APP}_lib_${LIB_MODE} STATIC ${SOURCE})	
endif()	

target_include_directories(${APP}_lib_${LIB_MODE} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${APP}_lib_${LIB_MODE} PUBLIC ${INCLUDE} lua_lib_${LIB_MODE})

add_library(${APP} ALIAS ${APP}_lib_${LIB_MODE})


#set_target_properties(${APP}_lib_${LIB_MODE} PROPERTIES VERSION "${PACKAGE_VERSION}")
#set_target_properties(${APP}_lib_${LIB_MODE} PROPERTIES OUTPUT_NAME "luabinding")
