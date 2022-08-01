set(APPMAIN main)
set(INCLUDE )
set(SOURCE
        main.cpp)

#set(THREADS_PREFER_PTHREAD_FLAG ON)
#find_package(Threads REQUIRED)

#target_link_libraries(${APPMAIN} ${LUA_LIB} ${LUABINDING_LIB})
#find_package(${LUA_LIB} ${LUABINDING_LIB} REQUIRED)
add_executable(${APPMAIN} ${SOURCE})
target_link_libraries(${APPMAIN} ${LUABINDING_LIB})

#target_link_libraries(${APPMAIN} Threads::Threads)
#stripped_build(${APPMAIN})
