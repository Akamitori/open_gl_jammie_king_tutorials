include(../../cmake_scripts/link_and_export_utilities.cmake)


add_and_export_library(OpenGL_Utilities TYPE SHARED FILES
        Shader.cpp
)

target_link_libraries(OpenGL_Utilities PRIVATE libglew_shared)