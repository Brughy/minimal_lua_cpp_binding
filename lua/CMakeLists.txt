set(LUA lua)
project(${LUA})

if(NOT DEFINED LUA_ENABLE_SHARED)
    set(LUA_ENABLE_SHARED 0)
endif ()

if(NOT DEFINED LUAVER)
    set(LUAVER 5.4.4)
endif ()

set(DIR "${CMAKE_CURRENT_LIST_DIR}/lua-${LUAVER}/src")

set(LUA_LIB_SRCS 
    "${DIR}/lapi.c"
    "${DIR}/lauxlib.c"
    "${DIR}/lbaselib.c"
    "${DIR}/lcode.c"
    "${DIR}/lcorolib.c"
    "${DIR}/lctype.c"
    "${DIR}/ldblib.c"
    "${DIR}/ldebug.c"
    "${DIR}/ldo.c"
    "${DIR}/ldump.c"
    "${DIR}/lfunc.c"
    "${DIR}/lgc.c"
    "${DIR}/linit.c"
    "${DIR}/liolib.c"
    "${DIR}/llex.c"
    "${DIR}/lmathlib.c"
    "${DIR}/lmem.c"
    "${DIR}/loadlib.c"
    "${DIR}/lobject.c"
    "${DIR}/lopcodes.c"
    "${DIR}/loslib.c"
    "${DIR}/lparser.c"
    "${DIR}/lstate.c"
    "${DIR}/lstring.c"
    "${DIR}/lstrlib.c"
    "${DIR}/ltable.c"
    "${DIR}/ltablib.c"
    "${DIR}/ltm.c"
    "${DIR}/lua.c"
    #"${DIR}/luac.c"
    "${DIR}/lundump.c"
    "${DIR}/lutf8lib.c"
    "${DIR}/lvm.c"
    "${DIR}/lzio.c"
)

set(LUA_LIB_INCLUDE 
    "${DIR}/lapi.h"
    "${DIR}/lauxlib.h"
    "${DIR}/lcode.h"
    "${DIR}/lctype.h"
    "${DIR}/ldebug.h"
    "${DIR}/ldo.h"
    "${DIR}/lfunc.h"
    "${DIR}/lgc.h"
    #"${DIR}/ljumptab.h"
    "${DIR}/llex.h"
    "${DIR}/llimits.h"
    "${DIR}/lmem.h"
    "${DIR}/lobject.h"
    "${DIR}/lopcodes.h"
    "${DIR}/lopnames.h"
    "${DIR}/lparser.h"
    "${DIR}/lprefix.h"
    "${DIR}/lstate.h"
    "${DIR}/lstring.h"
    "${DIR}/ltable.h"
    "${DIR}/ltm.h"
    "${DIR}/luaconf.h"
    "${DIR}/lua.h"
    "${DIR}/lualib.h"
    "${DIR}/lundump.h"
    "${DIR}/lvm.h"
    "${DIR}/lzio.h"
)

set(INCLUDE ${LUA_LIB_INCLUDE})
set(SOURCE  ${LUA_LIB_SRCS})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# 1. lua library
if(LUA_ENABLE_SHARED EQUAL 1)
	set(LIB_MODE "shared")
	add_library(${LUA}_lib_${LIB_MODE} SHARED ${SOURCE})
else()
	set(LIB_MODE "static")
	add_library(${LUA}_lib_${LIB_MODE} STATIC ${SOURCE})	
endif()	

target_link_libraries(${LUA}_lib_${LIB_MODE} PUBLIC ${INCLUDE} dl m)
add_library(lua ALIAS ${LUA}_lib_${LIB_MODE})
#set_target_properties(${LUA}_lib_${LIB_MODE} PROPERTIES VERSION "${PACKAGE_VERSION}")
#set_target_properties(${LUA}_lib_${LIB_MODE} PROPERTIES OUTPUT_NAME "lua")

# 2. lua interpreter
link_directories (${LUA_BINARY_DIR})
add_executable (lua_interpreter ${DIR}/lua.c)
target_link_libraries (lua_interpreter ${LUA}_lib_${LIB_MODE} )

# 3. lua compiler
#link_directories (${LUA_BINARY_DIR})
#add_executable (lua_compiler ${DIR}/luac.c)
#target_link_libraries (lua_compiler ${LUA}_lib_${LIB_MODE})
