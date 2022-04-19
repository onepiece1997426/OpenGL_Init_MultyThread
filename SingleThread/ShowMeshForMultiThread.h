#include <MyOpenGL.h>

//大概率不能用显示列表了 直接画顶点和mesh吧
//multy thread
#include <thread>
#include <mutex>

//该类为了实验多线程中，在另一个线程修改mesh之后如何画图
class ShowMeshForMultiThread:public MyOpenGLShowMesh
{
public:
    ShowMeshForMultiThread(){}

    void InitGLWindowForMultyThread(int argc, char** argv);

    //修改mesh
    void UpdateMeshForMultiThread(std::string meshFile);

};
