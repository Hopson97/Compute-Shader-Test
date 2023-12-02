cmake_minimum_required(VERSION 3.10)

#Set up project
project(
    dsa_practice 
    VERSION 1.0
)

include(${CMAKE_BINARY_DIR}/../conanbuildinfo.cmake)
conan_basic_setup()


#Set executable
add_executable(${PROJECT_NAME}     
    src/main.cpp
    src/GLDebugEnable.cpp
    src/GUI.cpp
    src/MeshGeneration.cpp
    src/Shader.cpp
    src/Util.cpp
)

#Set C++17
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_EXTENSIONS OFF)

#Set flags
if(MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "/O2")
    set(CMAKE_CXX_FLAGS_RELEASE "/Ox")
  	target_compile_options(${PROJECT_NAME} PRIVATE 
    	/W4)
else()
  	target_compile_options(${PROJECT_NAME} PRIVATE 
		-Wall -Wextra -pedantic)		
endif()


#Find libraries
find_package(Threads)

add_subdirectory(deps)
target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    deps
)

#Finally
target_link_libraries(${PROJECT_NAME} 
    glad 
    imgui_sfml
    Threads::Threads 
    ${CMAKE_DL_LIBS}
    ${CONAN_LIBS}
)
