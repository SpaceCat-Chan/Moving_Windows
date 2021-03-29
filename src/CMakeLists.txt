add_executable(Mover main.cpp Window/Window.cpp)

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

target_include_directories(Mover PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(Mover PRIVATE ${SDL2_LIBRARIES} OpenGL::GL GLEW::GLEW)
