# Base options
TEMPLATE = app
LANGUAGE  = C++

# QT modules
QT += opengl

# Executable name
TARGET = trimesh_qt

# Directories
DESTDIR = DESTDIR = ../distrib


# Lib headers
INCLUDEPATH += .
INCLUDEPATH += ../../..
INCLUDEPATH += /media/yangxuyuan/Planetarian/UAV_3DReconstruction/UAV_Code/v202006051741/server/code/dense/meshlab_merge/vcglib-master
INCLUDEPATH += /media/yangxuyuan/Planetarian/UAV_3DReconstruction/UAV_Code/v202006051741/server/code/dense/meshlab_merge/vcglib-master/wrap

# Lib sources
SOURCES += /media/yangxuyuan/Planetarian/UAV_3DReconstruction/UAV_Code/v202006051741/server/code/dense/meshlab_merge/vcglib-master/wrap/ply/plylib.cpp
SOURCES += /media/yangxuyuan/Planetarian/UAV_3DReconstruction/UAV_Code/v202006051741/server/code/dense/meshlab_merge/vcglib-master/wrap/gui/trackball.cpp
SOURCES += /media/yangxuyuan/Planetarian/UAV_3DReconstruction/UAV_Code/v202006051741/server/code/dense/meshlab_merge/vcglib-master/wrap/gui/trackmode.cpp


# Compile glew
DEFINES += GLEW_STATIC
INCLUDEPATH += /media/yangxuyuan/Planetarian/UAV_3DReconstruction/UAV_Code/v202006051741/server/code/dense/meshlab_merge/glew-1.7.0/include
SOURCES += /media/yangxuyuan/Planetarian/UAV_3DReconstruction/UAV_Code/v202006051741/server/code/dense/meshlab_merge/glew-1.7.0/src/glew.c


# Input
HEADERS += mainwindow.h
HEADERS += glarea.h
HEADERS += testOpenGL.h

SOURCES += main.cpp
SOURCES += mainwindow.cpp
SOURCES += glarea.cpp
SOURCES += testOpenGL.cpp

FORMS += mainwindow.ui

LIBS += -lGL -lglut -lGLU
