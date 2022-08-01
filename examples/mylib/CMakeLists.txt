set(APP mylib)
project(${APP})

if(NOT DEFINED LUA_ENABLE_SHARED)
    set(LUA_ENABLE_SHARED 0)
endif ()

if(LUA_ENABLE_SHARED EQUAL 1)
	set(LIB_MODE "shared")
else()
	set(LIB_MODE "static")	
endif()	

set(LIB_SRCS 
    "${CMAKE_CURRENT_LIST_DIR}/mylib.cpp"
)

set(LIB_INCLUDE 
    "${CMAKE_CURRENT_LIST_DIR}/mylib.h"
)

set(INCLUDE ${LIB_INCLUDE})
set(SOURCE  ${LIB_SRCS})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

#set(LIB_MODE "shared")
add_library(${APP} SHARED ${SOURCE} ${INCLUDE})

target_include_directories(${APP} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${APP} PUBLIC ${INCLUDE} lua_lib_${LIB_MODE})


