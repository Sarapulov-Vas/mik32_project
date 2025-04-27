# Setting up app build
set(APP_NAME ${CMAKE_PROJECT_NAME})

# Getting all files and directory in user directory
file(GLOB_RECURSE USER_DIR_ITEMS LIST_DIRECTORIES TRUE CONFIGURE_DEPENDS "user/*")

# Getting .c file and subdirectory
foreach(USER_DIR_ITEM ${USER_DIR_ITEMS})
    if (IS_DIRECTORY ${USER_DIR_ITEM})
        list(APPEND USER_SUBDIRECTORY ${USER_DIR_ITEM})
    elseif("${USER_DIR_ITEM}" MATCHES ".*(.c|.cpp|.s|.S|.asm)$")
        list(APPEND USER_SOURCES ${USER_DIR_ITEM})
    endif()
endforeach()

add_executable(${APP_NAME}
    crt.S
    main.c
    ${USER_SOURCES}
)

target_include_directories(${APP_NAME}
    PRIVATE
        .
        user
        ${USER_SUBDIRECTORY}
)

target_link_libraries(${APP_NAME}
    PRIVATE
        mik32_hal
        m
)

target_link_directories(${APP_NAME}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/hardware/mik32v2-shared/ldscripts
        .
)

# Convert output to hex and binary
add_custom_command(TARGET ${APP_NAME}
    POST_BUILD
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMAND ${TOOLCHAIN_PREFIX}-objcopy ARGS -O binary ${APP_NAME}${CMAKE_EXECUTABLE_SUFFIX_C} ${APP_NAME}.bin
    COMMAND ${TOOLCHAIN_PREFIX}-objcopy ARGS -O ihex ${APP_NAME}${CMAKE_EXECUTABLE_SUFFIX_C} ${APP_NAME}.hex
)