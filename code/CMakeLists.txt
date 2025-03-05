cmake_minimum_required(VERSION 3.17)
project(SolarSystem)

set(CMAKE_CXX_STANDARD 14)

if(WIN32)
    set(CMAKE_CXX_FLAGS "-static")
    include_directories(${PROJECT_SOURCE_DIR}/include)

    link_directories(cmake-build-debug/lib/windows)

    add_executable(CG_Project_SolarSystem ${PROJECT_SOURCE_DIR}/src/main.cpp ${PROJECT_SOURCE_DIR}/src/glad.c
            ${PROJECT_SOURCE_DIR}/src/Shaders.cpp ${PROJECT_SOURCE_DIR}/src/Camera.cpp ${PROJECT_SOURCE_DIR}/src/Sphere.cpp
            ${PROJECT_SOURCE_DIR}/src/TriMesh.cpp             src/InitShader.cpp
            ${PROJECT_SOURCE_DIR}/src/MeshPainter.cpp
    )

    target_link_libraries(CG_Project_SolarSystem glfw3)
endif()

if(APPLE)
    set(GLEW_H /usr/local/Cellar/glew/2.2.0/include)
    set(GLFW_H /usr/local/Cellar/glfw/3.3.2/include)
    set(GLAD_H /usr/local/include)
    set(KH_H /usr/local/include/KHR)
    include_directories(${GLEW_H} ${GLFW_H} ${GLAD_H} ${KH_H} ${PROJECT_SOURCE_DIR}/include)

# 添加目标链接
    set(GLEW_LINK /usr/local/Cellar/glew/2.2.0/lib/libGLEW.2.2.dylib)
    set(GLFW_LINK /usr/local/Cellar/glfw/3.3.2/lib/libglfw.3.dylib)
    link_libraries(${OPENGL} ${GLEW_LINK} ${GLFW_LINK})

# 执行编译命令
    set(SOURCE_FILES src/glad.c src/main.cpp src/Shaders.cpp src/Camera.cpp src/Sphere.cpp src/skyBox.h
            src/TriMesh.cpp
            src/TriMesh.h)

    add_executable(CG_Project_SolarSystem ${SOURCE_FILES})

    target_link_libraries(CG_Project_SolarSystem "-framework OpenGL")
    target_link_libraries(CG_Project_SolarSystem "-framework GLUT")
endif()