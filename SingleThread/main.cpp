#include "header.h"
#include "MyOpenGL.h"
#include "MeshWithCorners.h"
#include "ShowMeshForMultiThread.h"
#include <thread>

void TestChangeMesh(ShowMeshForMultiThread& showMeshMT)
{
    sleep(5);
    showMeshMT.UpdateMeshForMultiThread("/media/yangxuyuan/Planetarian/UAV_3DReconstruction/Samples/Guangdong_ridar/gdLidar_cut.ply");
    std::cout << "finish load" << std::endl;
    //draw
    //myOpenGLShwoMesh.UpdateGLMesh();

}

int main(int argc, char *argv[])
{

    string meshFile = "/media/yangxuyuan/Planetarian/UAV_3DReconstruction/Samples/xs/TestMesh/invalidAline/Scene_L0_B1_R68_dense_sample.ply";

   MyOpenGLShowMesh showMesh ;
   showMesh.InitGLWindow(argc,argv);
   showMesh.UpdateMesh(meshFile);
   showMesh.GLMainLoop();

    ShowMeshForMultiThread showMeshMT;
    showMeshMT.InitGLWindowForMultyThread(argc,argv);
    showMeshMT.UpdateMeshForMultiThread(meshFile);
    thread t(TestChangeMesh,ref(showMeshMT));
    t.detach();
    showMeshMT.GLMainLoop();


//    string rangeFile = "/media/yangxuyuan/Planetarian/UAV_3DReconstruction/Samples/xs/TestMesh/invalidAline/range_1.res";
//    MeshWithCorner meshWithCorner;
//    meshWithCorner.InitGLWindowWithMouse(argc,argv);

//    meshWithCorner.UpdateMesh(meshFile,rangeFile);
//    meshWithCorner.GLMainLoop();


    return 0;
}
