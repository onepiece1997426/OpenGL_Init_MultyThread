//https://www.freesion.com/article/2688454116/
//https://docs.microsoft.com/zh-cn/windows/win32/opengl/gluproject?redirectedfrom=MSDN
//https://blog.csdn.net/hjq376247328/article/details/49387011
//从代码上讲，我们使用glVertex3f设定的坐标值实际上是局部坐标系下的值
/*
 * 1.世界系
//这个阶段的变换，主要包括 glTranslae （平移）、
glScale （缩放）、
glRotate （旋转）。
这些变换针对向极坐标系原点操作,将相机原点移动到世界系下的不同位置，应该是对世界系中的模型进行操作的，而不是动相机
在经过这样一系列变换之后，
局部坐标系上的某个点P0(x0, y0, z0, 1)，会被摆放到世界坐标系上的P1(x1, y1, z1, 1)点
*/
/*
 *
    2. 世界系->相机系（正交/摄影）
    //gluLookAt(eye, center, up)  将摄像机摆放在了世界系下eye的位置，指定相机上方up，视线指向center。


*/

/*
 *
    3. 相机系->投影系（正交/摄影）

     正交投影在OpenGL中使用 glOrtho(left,  right, bottom, top,  near, far); 来进行设置。
    一般设定透视投影的方法有两种：
        3.1、gluPerspective(theta, aspect, near, far)
        3.2、glFrustum(left, right, bottom, top, near, far)
*/

/*
    4.规范化
    将原来的观察体，映射到规范化立方体的过程，就是规范化
    相当于将模型投影之后得到的平面所放到单位立方体中

*/
/*
    5.设置视口
     glViewport(x,y,  width, height)，
    调用这个方法之后，会将照片的左下角摆放在(x,y)点，并将其缩放，
    使得原来单位大小的图片，放大到宽为 width，高为 height。
*/


#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include<algorithm>

//#include <GL\freeglut.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

/// vcg imports
#include <vcg/complex/complex.h>
#include <vcg/complex/algorithms/update/bounding.h>
#include <vcg/complex/algorithms/update/normal.h>
#include <vcg/complex/algorithms/create/platonic.h>
#include <vcg/simplex/face/pos.h>
#include<vcg/complex/complex.h>
#include<wrap/io_trimesh/import_obj.h>
#include<wrap/io_trimesh/import_ply.h>
#include<vcg/complex/algorithms/update/topology.h>
#include<vcg/complex/algorithms/update/normal.h>
#include<vcg/space/box3.h>


class MyVertex; class MyEdge; class MyFace;
struct MyUsedTypes : public vcg::UsedTypes<vcg::Use<MyVertex>   ::AsVertexType,
                                           vcg::Use<MyEdge>     ::AsEdgeType,
                                           vcg::Use<MyFace>     ::AsFaceType>{};

class MyVertex  : public vcg::Vertex< MyUsedTypes, vcg::vertex::Coord3f, vcg::vertex::Normal3f, vcg::vertex::BitFlags  >{};
class MyFace    : public vcg::Face<   MyUsedTypes, vcg::face::FFAdj,  vcg::face::VertexRef, vcg::face::BitFlags > {};
class MyEdge    : public vcg::Edge<   MyUsedTypes> {};

class MyMesh    : public vcg::tri::TriMesh< std::vector<MyVertex>, std::vector<MyFace> , std::vector<MyEdge>  > {};

class MyVertex0  : public vcg::Vertex< MyUsedTypes, vcg::vertex::Coord3f, vcg::vertex::BitFlags  >{};
class MyVertex1  : public vcg::Vertex< MyUsedTypes, vcg::vertex::Coord3f, vcg::vertex::Normal3f, vcg::vertex::BitFlags  >{};
class MyVertex2  : public vcg::Vertex< MyUsedTypes, vcg::vertex::Coord3f, vcg::vertex::Color4b, vcg::vertex::CurvatureDirf,
                                                    vcg::vertex::Qualityf, vcg::vertex::Normal3f, vcg::vertex::BitFlags  >{};



//bool LoadMesh(string fileName);
//bool InitGLWindow(int argc, char** argv);
//bool InitGLMesh();
//void UpdateMesh(string fileName);
//void GLMainLoop();


class MyOpenGLShowMesh
{
public:
    MyMesh* myMeshData;
    vcg::Point3f modeAvePose; //use to translate

    MyOpenGLShowMesh()
    {
        myMeshData = new MyMesh();
    }

    bool LoadMesh(string fileName);
    bool InitGLWindow(int argc, char** argv);
    bool InitGLMesh();
    void UpdateMesh(string fileName);
    void GLMainLoop();
    void UpdateGLMesh();

    void StrengthPoints(std::vector<vcg::Point3f>& vec_Points); //强调的点

};

