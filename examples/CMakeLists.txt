function(stripped_build TARGET_APP)

    add_custom_command(
            TARGET ${TARGET_APP}
            POST_BUILD
            COMMAND cp ${TARGET_APP} ${TARGET_APP}-s
            COMMAND strip -s ${TARGET_APP}-s
            COMMAND mkdir -p ${DEST_BIN}
            COMMAND cp ${TARGET_APP}-s ${DEST_BIN}
    )
    set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${TARGET_APP}-s)
endfunction()

add_subdirectory(mylib)
add_subdirectory(dir0)

