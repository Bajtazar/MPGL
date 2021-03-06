cmake_minimum_required(VERSION 3.17)

project(MPGL VERSION 0.4 LANGUAGES CXX DESCRIPTION "Modern and Precise Graphics Library")

include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(MPGL_INSTALL "Installs the MPGL library" ON)

set(MPGL_SOURCES ${PROJECT_SOURCE_DIR}/src/MPGL)

add_library(${PROJECT_NAME} STATIC
    ${MPGL_SOURCES}/Exceptions/ShaderLibraryInvalidShadersException.cpp
    ${MPGL_SOURCES}/Collections/Image.cpp
    ${MPGL_SOURCES}/Mathematics/FFT.cpp
    ${MPGL_SOURCES}/Mathematics/FastFunctions.cpp
    ${MPGL_SOURCES}/Utility/StringAlgorithm.cpp
    ${MPGL_SOURCES}/IO/FileIO.cpp
    ${MPGL_SOURCES}/IO/ImageLoading/JPEGLoader.cpp
    ${MPGL_SOURCES}/IO/ImageLoading/ImageLoader.cpp
    ${MPGL_SOURCES}/IO/ImageLoading/BMPLoader.cpp
    ${MPGL_SOURCES}/IO/ImageLoading/PNGLoader.cpp
    ${MPGL_SOURCES}/Core/Figures/Angular.cpp
    ${MPGL_SOURCES}/Core/Figures/ResizableAngular.cpp
    ${MPGL_SOURCES}/Core/Figures/Elliptic.cpp
    ${MPGL_SOURCES}/Core/Figures/Figure.cpp
    ${MPGL_SOURCES}/Core/Figures/Primitives/Points.cpp
    ${MPGL_SOURCES}/Core/Figures/Primitives/LineStrip.cpp
    ${MPGL_SOURCES}/Core/Figures/Primitives/Triangle.cpp
    ${MPGL_SOURCES}/Core/Figures/Primitives/Ring.cpp
    ${MPGL_SOURCES}/Core/Figures/Primitives/Polygon.cpp
    ${MPGL_SOURCES}/Core/Figures/Primitives/Tetragon.cpp
    ${MPGL_SOURCES}/Core/Figures/Primitives/LineLoop.cpp
    ${MPGL_SOURCES}/Core/Figures/Primitives/Line.cpp
    ${MPGL_SOURCES}/Core/Figures/Primitives/Ellipse.cpp
    ${MPGL_SOURCES}/Core/Windows/WindowPlatform.cpp
    ${MPGL_SOURCES}/Core/Windows/Window.cpp
    ${MPGL_SOURCES}/Core/Windows/RenderWindow.cpp
    ${MPGL_SOURCES}/Core/Windows/RenderingAdapter.cpp
    ${MPGL_SOURCES}/Core/Text/Font.cpp
    ${MPGL_SOURCES}/Core/Text/VectorizedGlyph.cpp
    ${MPGL_SOURCES}/Core/Text/GlyphSprite.cpp
    ${MPGL_SOURCES}/Core/Text/TTFLoader.cpp
    ${MPGL_SOURCES}/Core/Text/Subfont.cpp
    ${MPGL_SOURCES}/Core/Text/Text.cpp
    ${MPGL_SOURCES}/Core/Text/FontComponents.cpp
    ${MPGL_SOURCES}/Core/Text/UTF-8.cpp
    ${MPGL_SOURCES}/Core/Text/FontRasterizer.cpp
    ${MPGL_SOURCES}/Core/Context/Context.cpp
    ${MPGL_SOURCES}/Core/Context/ShadersContext.cpp
    ${MPGL_SOURCES}/Core/Context/Buffers/Helpers.cpp
    ${MPGL_SOURCES}/Core/Context/Buffers/VertexBuffer.cpp
    ${MPGL_SOURCES}/Core/Context/Buffers/FrameBuffer.cpp
    ${MPGL_SOURCES}/Core/Context/Buffers/TextureBuffer.cpp
    ${MPGL_SOURCES}/Core/Context/Buffers/VertexArray.cpp
    ${MPGL_SOURCES}/Core/Context/Buffers/ElementArrayBuffer.cpp
    ${MPGL_SOURCES}/Core/Textures/Texture.cpp
    ${MPGL_SOURCES}/Core/Textures/RingSprite.cpp
    ${MPGL_SOURCES}/Core/Textures/EllipseSprite.cpp
    ${MPGL_SOURCES}/Core/Textures/TexturePack.cpp
    ${MPGL_SOURCES}/Core/Textures/Sprite.cpp
    ${MPGL_SOURCES}/Core/Textures/TextureLoader.cpp
    ${MPGL_SOURCES}/Core/Textures/Texturable.cpp
    ${MPGL_SOURCES}/Core/Textures/EllipticSprite.cpp
    ${MPGL_SOURCES}/Core/Textures/ShadeableSprite.cpp
    ${MPGL_SOURCES}/Core/Shaders/Shadeable.cpp
    ${MPGL_SOURCES}/Core/Shaders/Shader.cpp
    ${MPGL_SOURCES}/Core/Shaders/ShaderLocation.cpp
    ${MPGL_SOURCES}/Core/Shaders/ShaderLibrary.cpp
    ${MPGL_SOURCES}/Core/Shaders/ShaderProgram.cpp
    ${MPGL_SOURCES}/Core/Layouts/DefaultLayout.cpp
    ${MPGL_SOURCES}/Core/Layouts/AnchoredLayout.cpp
    ${MPGL_SOURCES}/Concurrency/Threadpool.cpp
    ${MPGL_SOURCES}/Compression/Checksums/CRC32.cpp
    ${MPGL_SOURCES}/Iterators/SafeIterator.cpp
    ${MPGL_SOURCES}/Events/Registers/ScreenTransformationRegister.cpp
    ${MPGL_SOURCES}/Events/Registers/TickRegister.cpp
    ${MPGL_SOURCES}/Events/Management/BasicWindowEventManager.cpp
)

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION 1)

target_compile_definitions(${PROJECT_NAME} PRIVATE "MPGL_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\"")

# configure_file(mpgl.pc.in mpgl.pc @ONLY)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_subdirectory(${PROJECT_SOURCE_DIR}/external/glfw)
add_subdirectory(${PROJECT_SOURCE_DIR}/external/glad)

SET(GLAD_API "gl=4.6" CACHE STRING "API type/version pairs, like \"gl=3.2,gles=\", no version means latest")

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
