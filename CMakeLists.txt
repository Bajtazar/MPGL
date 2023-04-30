cmake_minimum_required(VERSION 3.17)

project(mpgl VERSION 0.4 LANGUAGES CXX DESCRIPTION "Modern and Precise Graphics Library")

include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(MPGL_INSTALL "Installs the MPGL library" ON)

file(GLOB_RECURSE MPGL_SRC ${PROJECT_SOURCE_DIR}/src/MPGL/*.cpp)

add_library(${PROJECT_NAME} STATIC ${MPGL_SRC})

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION 1)

target_compile_definitions(${PROJECT_NAME} PRIVATE "MPGL_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\"")

set(MPGL_RELEASE_FLAGS "-Wall;-Wextra;-Werror;-pedantic;-fPIE;-O2;-fcoroutines")
target_compile_options(${PROJECT_NAME} PUBLIC ${MPGL_RELEASE_FLAGS})

# configure_file(mpgl.pc.in mpgl.pc @ONLY)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_subdirectory(${PROJECT_SOURCE_DIR}/external/glfw)
add_subdirectory(${PROJECT_SOURCE_DIR}/external/glad)

set(GLAD_API "gl=4.6" CACHE STRING "API type/version pairs, like \"gl=3.2,gles=\", no version means latest")

target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/external/glfw/include)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${GLAD_OUT_DIR}/include)

target_link_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/external/glfw/src)

target_link_directories(${PROJECT_NAME}
    PUBLIC ${GLAD_OUT_DIR}/src)

target_link_libraries(${PROJECT_NAME} glfw glad)

if(MPGL_INSTALL)
    install(TARGETS ${PROJECT_NAME}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
    # install(FILES ${CMAKE_BINARY_DIR}/mpgl.pc
    #     DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)
endif()
