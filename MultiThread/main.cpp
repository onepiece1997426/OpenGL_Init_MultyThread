/****************************************************************************
 * VCGLib                                                            o o     *
 * Visual and Computer Graphics Library                            o     o   *
 *                                                                _   O  _   *
 * Copyright(C) 2007                                                \/)\/    *
 * Visual Computing Lab                                            /\/|      *
 * ISTI - Italian National Research Council                           |      *
 *                                                                    \      *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 2 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 *                                                                           *
 ****************************************************************************/
/****************************************************************************
  History

$Log: not supported by cvs2svn $

****************************************************************************/

/**
 * Minimal QT trimesh viewer
 *
 * This sample shows how to use togheter:
 * - the Opengl module in QT using the designer
 * - the trimesh loading and initialization
 * - basic usage of the default manipulators (the "Trackball")
 */

//#include <QApplication>
//#include "mainwindow.h"
#include <thread>
#include "time.h"
#include "stdio.h"

#include "testOpenGL.h"

//void testThread(MainWindow* mw)
//{
//    sleep(5);
//    mw->ui.glArea->ChangeMesh();   //另一个线程内可以改这个mesh，但是无法在不同线程处刷新GL
//    //因此只能写一个信号函数，调用该信号函数表示mesh修改，在GL线程收到信号后进行调用刷新GL的函数
//    std::cout << "changed mesh" << std::endl;


//}

int main(int argc, char** argv)
{
    //  QApplication app(argc, argv);
    //  MainWindow *mw = new MainWindow;
    //  mw->show(); //窗口线程开启，会在exec中循环

    //  thread T(testThread,ref(mw));
    //  T.detach();

    //  app.exec(); //结束之前会在这里循环

    //glutInit(&argc,argv);
    UseGLShowMesh(argc,argv);


    return 0;
}
