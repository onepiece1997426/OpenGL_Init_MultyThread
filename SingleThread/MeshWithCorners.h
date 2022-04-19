//brief
/*
 *  认为模型几乎在同一个平面上
    通过OpenGL显示模型，并根据corner文件绘制不同区域的角点
    根据鼠标在屏幕下的位置求出鼠标在世界系下的XY值
    单击鼠标时返回该区域编号
*/

#include "MyOpenGL.h"

class MeshWithCorner: public MyOpenGLShowMesh
{
public:
    std::map<uint,vcg::Point4f> mp_XminXmaxYminYmax;  //from range file ,加载时会平移avePos个值
    //认为Z为高度
    std::map<uint,float> mp_AveZ;  //from mesh， correct with range，use to calculate mouse positon in world pos
    std::map<uint,uint> mp_rangePointSize; //每个区域的点的个数

    void UpdateMesh(string meshFile, string rangeFile);

    bool ReadRangeFromFileAndUpdateInfo(std::string fileName); //读文件并且更新mp_AveZ，mp_hasPointsInRange
    void UpdateRangePoints(); //update gl to draw Points
    void InitGLWindowWithMouse(int argc, char** argv); //带鼠标事件

    static void ShowMouseRangeIndex(); //显示鼠标所在位置的区域编号

};



