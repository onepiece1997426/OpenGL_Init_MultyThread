#include "MeshWithCorners.h"

int mouseX_screen = 0;
int mouseY_screen = 0;

std::map<uint,vcg::Point4f> gmp_XminXmaxYminYmax;
std::map<uint,float> gmp_AveZ;  //from mesh， correct with range，use to calculate mouse positon in world pos
std::map<uint,uint> gmp_rangePointSize; //每个区域的点的个数
std::map<uint,vcg::Point4f> gmp_ProjectionRange;//每个区域投影到像素点的位置Xmin Xmax Ymin Ymax (pixel)

GLuint showRangePointList;



//获取鼠标
void MyGetMouse(int button,int state,int x,int y)
{
    mouseX_screen = x;
    mouseY_screen = y;

    switch(button)
    {
    case GLUT_LEFT_BUTTON:
        if(state==GLUT_DOWN)
            MeshWithCorner::ShowMouseRangeIndex();
        break;
    default:
        break;
    }
}

//MeshWith Corner----------------

void MeshWithCorner::UpdateMesh(string meshFile, string rangeFile)
{
    MyOpenGLShowMesh::UpdateMesh(meshFile);
    ReadRangeFromFileAndUpdateInfo(rangeFile);

    //是否需要强调边界点的展示
    std::vector<vcg::Point3f> vec_boundPoints;
    vec_boundPoints.reserve(4 * mp_XminXmaxYminYmax.size());
    for(auto iter_bound = mp_XminXmaxYminYmax.begin();iter_bound != mp_XminXmaxYminYmax.end();iter_bound++)
    {
        uint idx = iter_bound->first;
        vec_boundPoints.push_back(vcg::Point3f(iter_bound->second.X(),iter_bound->second.Z(),mp_AveZ[idx]));
        vec_boundPoints.push_back(vcg::Point3f(iter_bound->second.X(),iter_bound->second.W(),mp_AveZ[idx]));
        vec_boundPoints.push_back(vcg::Point3f(iter_bound->second.Y(),iter_bound->second.Z(),mp_AveZ[idx]));
        vec_boundPoints.push_back(vcg::Point3f(iter_bound->second.Y(),iter_bound->second.W(),mp_AveZ[idx]));
    }
    StrengthPoints(vec_boundPoints);

}

bool MeshWithCorner::ReadRangeFromFileAndUpdateInfo(std::string fileName)
{
    gmp_XminXmaxYminYmax.clear();
    gmp_AveZ.clear();
    gmp_ProjectionRange.clear();
    gmp_rangePointSize.clear();
    mp_XminXmaxYminYmax.clear();
    mp_AveZ.clear();
    mp_rangePointSize.clear();

    if(myMeshData == nullptr || myMeshData->VertexNumber() == 0)
    {
        std::cout << "please inint mesh first" << std::endl;
        return false;
    }
    //ID Xmin Xmax Ymin Ymax
    ifstream ifs(fileName);
    string line = "";
    while(getline(ifs,line))
    {
        stringstream ss(line);
        std::vector<string> vec_tmp;
        std::string tmp;
        while(getline(ss,tmp,' '))
            vec_tmp.push_back(tmp);

        uint id = std::atoi( vec_tmp[0].c_str());
        vcg::Point4f bound(std::atof( vec_tmp[1].c_str()),std::atof( vec_tmp[2].c_str()),std::atof( vec_tmp[3].c_str()),std::atof( vec_tmp[4].c_str()));

        //translate avePOs
        bound.X() -= modeAvePose.X();
        bound.Y() -= modeAvePose.X();
        bound.Z() -= modeAvePose.Y();
        bound.W() -= modeAvePose.Y();

        mp_XminXmaxYminYmax[id] = bound;
        mp_rangePointSize[id] = 0;
        mp_AveZ[id] = 0.0f;
    }


    //update mp_AveZ
    //可以优化 懒得改了
    std::vector<MyMesh::VertexType>& vec_vertexes = myMeshData->vert;
    for (auto iter_vert = vec_vertexes.begin(); iter_vert != vec_vertexes.end();iter_vert++)
    {
        for(auto iter_bound = mp_XminXmaxYminYmax.begin();iter_bound != mp_XminXmaxYminYmax.end(); iter_bound++)
        {
            vcg::Point4f& bound = iter_bound->second;
            if(iter_vert->P().X() > bound.X() && iter_vert->P().X() < bound.Y()
                    && iter_vert->P().Y() > bound.Z() && iter_vert->P().Y() < bound.W())
            {
                uint idx = iter_bound->first;
                mp_rangePointSize[idx]++;
                mp_AveZ[idx] = mp_AveZ[idx] * (float( mp_rangePointSize[idx] - 1) /  mp_rangePointSize[idx]) + iter_vert->P().Z() /  mp_rangePointSize[idx];
                break;
            }
        }
    }

    //project bound points to screen
//    GLint viewport[4];
//    GLdouble mvmatrix[16], projmatrix[16];

//    glGetIntegerv(GL_VIEWPORT, viewport);
//    glGetDoublev(GL_MODELVIEW_MATRIX, mvmatrix);
//    glGetDoublev(GL_PROJECTION_MATRIX, projmatrix);

//    GLdouble identity[16] = {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};


//    for(auto iter_bound = mp_XminXmaxYminYmax.begin();iter_bound != mp_XminXmaxYminYmax.end(); iter_bound++)
//    {
//        if(mp_rangePointSize[iter_bound->first] == 0)
//            continue;
//        vcg::Point4f& bound = iter_bound->second;
//        GLdouble wx, wy, wz;


//        //wrong here
//        //use gluProject after reshape
//        //GLint err = gluProject(double(bound.X()),double(bound.Z()),double(mp_AveZ[iter_bound->first]),identity,projmatrix,viewport,&wx,&wy,&wz);
////        gluProject(bound.X(),bound.W(),mp_AveZ[iter_bound->first],mvmatrix,projmatrix,viewport,&wx,&wy,&wz);
////        gluProject(bound.Y(),bound.Z(),mp_AveZ[iter_bound->first],mvmatrix,projmatrix,viewport,&wx,&wy,&wz);
////        gluProject(bound.Y(),bound.W(),mp_AveZ[iter_bound->first],mvmatrix,projmatrix,viewport,&wx,&wy,&wz);

//        //        if(err == GL_TRUE)
////            int b = 0;
//        int a = 0;


//    }

    gmp_XminXmaxYminYmax = this->mp_XminXmaxYminYmax;
    gmp_AveZ = this->mp_AveZ;
    gmp_rangePointSize = this->mp_rangePointSize;

    return true;
}

void MeshWithCorner::InitGLWindowWithMouse(int argc, char **argv)
{
    InitGLWindow(argc,argv);
    //
    glutMouseFunc(MyGetMouse);
}

void MeshWithCorner::ShowMouseRangeIndex()
{
    //窗口左上角原点，右下角为正方向
    //但是图像系(即gluProject之后的坐标)是左下角为原点，右上角为正方向
    //因此后续需要做  viewport[3] - mouseY_screen才是鼠标在左下角系的坐标
    //viewport[4]:窗口左上角(0,0),窗口右下角(1024,768),即窗口原点和窗口尺寸
    std::cout << "mouse: ( " << mouseX_screen << " , " << mouseY_screen << " )" << std::endl;

    //Calculate Mouse word X,Y in each average position
    GLint viewport[4];
    GLdouble mvmatrix[16], projmatrix[16];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glGetDoublev(GL_MODELVIEW_MATRIX, mvmatrix);
    glGetDoublev(GL_PROJECTION_MATRIX, projmatrix);

    float depthF = 0.0f;
    double depth = 0.0;
    glReadPixels(mouseX_screen,mouseY_screen,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&depthF);
    depth = static_cast<double>(depthF);
//    GLdouble x, y, z;
//    gluUnProject(mouseX_screen,mouseY_screen,static_cast<double>(depth),mvmatrix,projmatrix,viewport,&x,&y,&z);
//    std::cout << x << " " << y << " " << z  << std::endl;
    gmp_ProjectionRange.clear();
    for(std::pair<uint,vcg::Point4f> pair : gmp_XminXmaxYminYmax)
    {
//        if(gmp_rangePointSize[pair.first] == 0)
//            continue;

        vcg::Point4d bound = vcg::Point4d(static_cast<double>(pair.second.X()) ,static_cast<double>(pair.second.Y()),static_cast<double>(pair.second.Z()),static_cast<double>(pair.second.W()));
        double aveZ = static_cast<double>(gmp_AveZ[pair.first]);
//        if(x > bound.X() && x < bound.Y() && y > bound.Z() && y < bound.W())
//        {
//            std::cout << "groupID: " << pair.first  << std::endl;
//            break;
//        }
        double x1,y1,z1;
        gluProject(bound.X(),bound.Z(),aveZ,mvmatrix,projmatrix,viewport,&x1,&y1,&z1);
        x1 *= (depth / z1);
        y1 *= (depth / z1);
        z1 = depth;

        double x2,y2,z2;
        gluProject(bound.X(),bound.W(),aveZ,mvmatrix,projmatrix,viewport,&x2,&y2,&z2);
        x2 *= (depth / z2);
        y2 *= (depth / z2);
        z2 = depth;

        double x3,y3,z3;
        gluProject(bound.Y(),bound.W(),aveZ,mvmatrix,projmatrix,viewport,&x3,&y3,&z3);
        x3 *= (depth / z3);
        y3 *= (depth / z3);
        z3 = depth;

        double x4,y4,z4;
        gluProject(bound.Y(),bound.Z(),aveZ,mvmatrix,projmatrix,viewport,&x4,&y4,&z4);
        x4 *= (depth / z4);
        y4 *= (depth / z4);
        z4 = depth;

//        std::cout << "range point: (" << x1 << "," <<y1 << ") (" << x2 << "," <<y2 << ")" << std::endl
//                  << "(" << x3 << "," <<y3 << ") (" << x4 << "," <<y4 << ")" << std::endl;

        //gmp_ProjectionRange[pair.first] = vcg::Point4f(x1,x2,y1,y2);

//        double xmax = max(x1,x2);
//        double xmin = min(x1,x2);
//        double ymax = max(y1,y2);
//        double ymin = min(y1,y2);
//        if(mouseX_screen > xmin && mouseX_screen < xmax && mouseY_screen > ymin && mouseY_screen < ymax)
//        {
//            std::cout << "groupID: " << pair.first  << std::endl;
//            break;
//        }

        Eigen::Vector3d p1(x1,y1,z1);
        Eigen::Vector3d p2(x2,y2,z2);
        Eigen::Vector3d p3(x3,y3,z3);
        Eigen::Vector3d p4(x4,y4,z4);
        Eigen::Vector3d p(mouseX_screen, viewport[3] - mouseY_screen,depth);

        //点在四边形内
        //顺时针叉乘同向
        bool sign = (((p1 - p).cross(p2 - p)).dot((p2 - p).cross(p3 - p)) > 0)
                && (((p2 - p).cross(p3 - p)).dot((p3 - p).cross(p4 - p)) > 0)
                && (((p3 - p).cross(p4 - p)).dot((p4 - p).cross(p1 - p)) > 0)
                && (((p4 - p).cross(p1 - p)).dot((p1 - p).cross(p2 - p)) > 0);
        if(sign)
        {
            std::cout << "groupID: " << pair.first  << std::endl;
            break;
        }
    }
    int a = 0;

}
