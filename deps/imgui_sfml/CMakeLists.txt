add_library(
    imgui_sfml
    imgui_impl_opengl3.cpp
    imgui-SFML.cpp
)   

target_compile_options(glad PRIVATE -fPIE)