cmake_minimum_required(VERSION 2.4)

project(VCG_OPENGL)


set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_FLAGS "-std=c++11")
set(CMAKE_CXX_FLAGS "-Wl,--no-undefined")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${CMAKE_CURRENT_SOURCE_DIR}")
SET( CMAKE_VERBOSE_MAKEFILE on )
set(CMAKE_CXX_FLAGS "-std=c++11 -o3 -lGL -lGLU -lglut")

aux_source_directory(./ src)
include_directories(./)
FILE(GLOB_RECURSE HeaderFile ./ *.h )


find_package(OpenGL REQUIRED)

include_directories(${OPENGL_INCLUDE_DIR})  

find_package(GLUT REQUIRED)  
include_directories(${GLUT_INCLUDE_DIR})  

message(${OPENGL_LIBRARIES})
message(${GLUT_LIBRARY})


set(VCG_DIR /media/yangxuyuan/Planetarian/UAV_3DReconstruction/UAV_Code/v202006051741/server/code/dense/meshlab_merge/vcglib-master)
include_directories(
        ${VCG_DIR}
)

#ply IO
#https://www.jianshu.com/p/a9e846b105ca
aux_source_directory(/media/yangxuyuan/Planetarian/UAV_3DReconstruction/UAV_Code/v202006051741/server/code/dense/meshlab_merge/vcglib-master/wrap/ply VCG_PLY)
file(
    GLOB VCG_PLY
        /media/yangxuyuan/Planetarian/UAV_3DReconstruction/UAV_Code/v202006051741/server/code/dense/meshlab_merge/vcglib-master/wrap/ply/plylib.cpp
)

add_executable(${PROJECT_NAME} ${src} ${VCG_PLY})


target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} -lpthread )
target_sources(${PROJECT_NAME} PUBLIC /media/yangxuyuan/Planetarian/UAV_3DReconstruction/UAV_Code/v202006051741/server/code/dense/meshlab_merge/vcglib-master/wrap/ply/plylib.cpp)
