cmake_minimum_required(VERSION 3.10)

#Set up project
project(
    compute_tests 
    VERSION 1.0
)

include("$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")

#Set executable
add_executable(${PROJECT_NAME}     

    src/main.cpp
    src/GUI.cpp
    src/Util.cpp

    src/Graphics/GLDebugEnable.cpp
    src/Graphics/Shader.cpp
    src/Graphics/Texture.cpp
    src/Graphics/VertexArray.cpp

    src/Applications/Application.cpp
    src/Applications/GameOfLife.cpp
    src/Applications/CubeCompute.cpp

)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_23)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_EXTENSIONS OFF)

target_compile_definitions(${PROJECT_NAME} PRIVATE GLM_ENABLE_EXPERIMENTAL)

if(MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "/O2")
    set(CMAKE_CXX_FLAGS_RELEASE "/Ox")
  	target_compile_options(${PROJECT_NAME} PRIVATE 
    	/W4)
else()
  	target_compile_options(${PROJECT_NAME} PRIVATE 
		-Wall -Wextra -pedantic)		
endif()

find_package(glm CONFIG REQUIRED)
find_package(SFML COMPONENTS system audio network window graphics CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

add_subdirectory(deps)
target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    deps
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    sfml-system sfml-audio sfml-network sfml-graphics sfml-window
    imgui::imgui
    glm::glm
    imgui_sfml
    glad 
)