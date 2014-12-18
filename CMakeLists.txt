#Copyright © 2014 Fred Mariowitsch Flügge <fmfluegge@gmail.com>
#This work is free. You can redistribute it and/or modify it under the
#terms of the Do What The Fuck You Want To Public License, Version 2,
#as published by Sam Hocevar. See http://www.wtfpl.net/ for more details.

cmake_minimum_required(VERSION 2.8)
PROJECT( fvis )

find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})
add_definitions(${GLUT_DEFINITIONS})

find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})

add_executable(fvis main.cpp)

target_link_libraries(fvis ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )
