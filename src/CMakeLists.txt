cmake_minimum_required(VERSION 2.8)

# Cloth simulation source
set(CLOTHSIM_VIEWER_SOURCE
    # Cloth simulation objects

    # Collision objects
    collision/sphere.cpp
    collision/plane.cpp

    # Application
    main.cpp
        Simulator.cpp

    # Miscellaneous
    # png.cpp
    misc/sphere_drawing.cpp
    misc/file_utils.cpp

    # Camera
    camera.cpp
        liquid.cpp liquid.h)

# Windows-only sources
if(WIN32)
list(APPEND CLOTHSIM_VIEWER_SOURCE
    # For get-opt
    misc/getopt.c
)
endif(WIN32)

#-------------------------------------------------------------------------------
# Embed resources
#-------------------------------------------------------------------------------

# embed_resources(CLOTHSIM_SHADERS
#   shaders/vert.glsl
#   shaders/normalFrag.glsl
#   shaders/wireframeFrag.glsl
# )

#-------------------------------------------------------------------------------
# Set definitions
#-------------------------------------------------------------------------------
add_definitions(${NANOGUI_EXTRA_DEFS})

#-------------------------------------------------------------------------------
# Set include directories
#-------------------------------------------------------------------------------
include_directories(
  ${CGL_INCLUDE_DIRS}
  ${FREETYPE_INCLUDE_DIRS}
  ${NANOGUI_EXTRA_INCS}
)

#-------------------------------------------------------------------------------
# Set link directories
#-------------------------------------------------------------------------------
link_directories(
  ${CGL_LIBRARY_DIRS}
  ${FREETYPE_LIBRARY_DIRS}
)

#-------------------------------------------------------------------------------
# Add executable
#-------------------------------------------------------------------------------
add_executable(liquidsim ${CLOTHSIM_VIEWER_SOURCE})

target_link_libraries(liquidsim
    CGL ${CGL_LIBRARIES}
    nanogui ${NANOGUI_EXTRA_LIBS}
    ${FREETYPE_LIBRARIES}
    ${CMAKE_THREADS_INIT}
)

#-------------------------------------------------------------------------------
# Platform-specific configurations for target
#-------------------------------------------------------------------------------
if(APPLE)
  set_property( TARGET liquidsim APPEND_STRING PROPERTY COMPILE_FLAGS
                "-Wno-deprecated-declarations -Wno-c++11-extensions")
endif(APPLE)

# Put executable in build directory root
set(EXECUTABLE_OUTPUT_PATH ..)

# Install to project root
install(TARGETS liquidsim DESTINATION ${ClothSim_SOURCE_DIR})
