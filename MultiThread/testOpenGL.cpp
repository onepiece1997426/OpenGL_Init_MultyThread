#include "testOpenGL.h"

//global varieties
CMesh mesh;
float xRotate = 0.0f;
float yRotate = 0.0f;
float zRotate = 0.0f;
float scale = 1.0;
bool showFace = true;
bool showWire = true;
bool showPoint = false;
GLuint showFaceList, showWireList, showPointList;
Point3f ave(0.0f,0.0f,0.0f);

float translateX = 0.0f;
float translateY = 0.0f;
float translateZ = 0.0f;


//-------openGL options
//初始化，所有东西都渲染上，后叙调用法的时候从每个list中拿
void initGL()
{

    //translate model
    std::vector<CMesh::VertexType>& vec_vertexes = mesh.vert;

    for (auto iter_vert = vec_vertexes.begin(); iter_vert != vec_vertexes.end();iter_vert++)
    {
         ave += iter_vert->P();
    }
    ave /= vec_vertexes.size();
    for (auto iter_vert = vec_vertexes.begin(); iter_vert != vec_vertexes.end();iter_vert++)
    {
         iter_vert->P() -= ave;
    }
    ave = Point3f(0,0,0);


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

    gluLookAt(ave.X(),ave.Y(),ave.Z() + 200.0f, ave.X(),ave.Y(),ave.Z(), 0.0f,1.0f,0.0f);

    glClearColor(0.3, 0.3, 0.3, 0.0);
    glClearDepth(1.0);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    // ------------------- Lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    // ------------------- Display List
    showFaceList = glGenLists(1);
    showWireList = glGenLists(1);
    showPointList = glGenLists(1);
    uint temp = mesh.face.size();
    // SHOW WIRE 可能是把这些东西的信息放到showWireList中，后续直接从这个里面调用已经渲染的东西
    glNewList(showWireList, GL_COMPILE);
    glDisable(GL_LIGHTING);
    glLineWidth(2.0f);
    glColor3f(0.1f, 0.1f, 0.1f);
    glBegin(GL_LINES);
//    std::vector<CMesh::EdgeType>& vec_edges = mesh.edge;
//    for (auto iter = vec_edges.begin();iter != vec_edges.end();iter++)
//    {
//        glVertex3d(iter->P(0).X(),iter->P(0).Y(),iter->P(0).Z());
//        glVertex3d(iter->P(1).X(),iter->P(1).Y(),iter->P(1).Z());
//    }
    std::vector<CMesh::FaceType>& vec_faces = mesh.face;
    for (auto iter_face = vec_faces.begin();iter_face != vec_faces.end();iter_face++)
    {
        for (int i = 0;i<3;i++)
        {
            for (int j = i+1;j<3;j++)
            {
                glVertex3d(iter_face->P(i).X(),iter_face->P(i).Y(),iter_face->P(i).Z());
                glVertex3d(iter_face->P(j).X(),iter_face->P(j).Y(),iter_face->P(j).Z());
            }
        }
    }
    glEnd();
    glEnable(GL_LIGHTING);
    glEndList();
    // SHOW POINT
    glNewList(showPointList, GL_COMPILE);
    glDisable(GL_LIGHTING);
    glLineWidth(3.0f);
    glColor3f(0.8f, 0.8f, 0.8f);
    glBegin(GL_POINTS);
    for (auto iter_vert = vec_vertexes.begin(); iter_vert != vec_vertexes.end();iter_vert++)
    {      
        glVertex3d(iter_vert->P().X(),iter_vert->P().Y(),iter_vert->P().Z());
    }
    glEnd();
    glEnable(GL_LIGHTING);
    glEndList();
    // SHOW FACE
    glNewList(showFaceList, GL_COMPILE);
    for (auto iter_face = vec_faces.begin();iter_face != vec_faces.end();iter_face++)
    {
        glBegin(GL_TRIANGLES);

        for (int i = 0;i<3;i++)
        {
            //glNormal3f(iter_face->N().X(),iter_face->N().Y(),iter_face->N().Z());
            glVertex3d(iter_face->P(i).X(),iter_face->P(i).Y(),iter_face->P(i).Z());
            //glVertex3f(mesh.point(*fv_it)[0], mesh.point(*fv_it)[1], mesh.point(*fv_it)[2]);
        }
        glEnd();
    }
    glEndList();
}


//reshape window
void myReshape(GLint w, GLint h)
{
    glViewport(0, 0, static_cast<GLsizei>(w), static_cast<GLsizei>(h));
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w > h)
        glOrtho(-static_cast<GLdouble>(w) / h, static_cast<GLdouble>(w) / h, -1.0, 1.0, -1.0, 1.0);
    else
        glOrtho(-1.0, 1.0, -static_cast<GLdouble>(h) / w, static_cast<GLdouble>(h) / w, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


void myIdle()
{
    xRotate += 0.5f;
    yRotate += 1.0f;
    zRotate += 1.5f;
    if (xRotate >= 360.0f)
        xRotate -= 360.0f;
    if (yRotate >= 360.0f)
        yRotate -= 360.0f;
    if (zRotate >= 360.0f)
        zRotate -= 360.0f;
    glutPostRedisplay();
}


void myKeyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case '1':
        showFace = !showFace;
        break;
    case '2':
        showWire = !showWire;
        break;
    case '3':
        showPoint = !showPoint;
        break;
    case 'w':
        translateY += 1.0;
        break;
    case 's':
        translateY -= 1.0;
        break;
    case 'd':
        translateX += 1.0;
        break;
    case 'a':
        translateX -= 1.0;
        break;
    default:
        break;
    }
    //std::cout << key << std::endl;
    glutPostRedisplay();
}

//reset model
void myDisplay()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glRotatef(xRotate, 1.0f, 0.0f, 0.0f);
    glRotatef(yRotate, 0.0f, 1.0f, 0.0f);
    glRotatef(zRotate, 0.0f, 0.0f, 1.0f);
    glScalef(scale, scale, scale);
    glTranslate(Point3f(translateX,translateY,translateZ));

    //切换之后，从每个list中拿出渲染的东西显示
    if (showFace)
        glCallList(showFaceList);
    if (showPoint)
        glCallList(showPointList);
    if (showWire)
        glCallList(showWireList);
    glutSwapBuffers();
}


//special key board movement
void mySpecial(int key, int x, int y) {
    switch (key)
    {
    case GLUT_KEY_UP:
        xRotate += 5.0f;
        break;
    case GLUT_KEY_DOWN:
        xRotate -= 5.0f;
        break;
    case GLUT_KEY_LEFT:
        yRotate += 5.0f;
        break;
    case GLUT_KEY_RIGHT:
        yRotate -= 5.0f;
        break;
    case GLUT_KEY_PAGE_UP:
        scale = scale + 0.02;
        break;
    case GLUT_KEY_PAGE_DOWN:
        scale = scale - 0.02;
        break;
    default:
        break;
    }
    glutPostRedisplay();
}


//----------------- VCG options

bool SetMesh(std::string fileName)
{
    int err=vcg::tri::io::ImporterPLY<CMesh>::Open(mesh,(fileName).c_str());
    return err == 0;
}

//another options
void UseGLShowMesh(int argc, char** argv)
{

    //set mesh
    if(!SetMesh("/media/yangxuyuan/Planetarian/UAV_3DReconstruction/Samples/xs/TestMesh/lossCorner/Scene_L0_B4_R940_dense_sample.ply"))
    {
        std::cout << "invalid load mesh" << std::endl;
        return ;
    }

    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(1024, 768);
    glutCreateWindow("Mesh Viewer");

    initGL(); //load mesh

    glutKeyboardFunc(&myKeyboard);
    glutSpecialFunc(&mySpecial);
    glutReshapeFunc(&myReshape);
    glutDisplayFunc(&myDisplay);
    glutMainLoop(); //loop window here

}


//int main(int argc, char** argv)
//{
//    //set mesh
//    if(!SetMesh("/media/yangxuyuan/Planetarian/UAV_3DReconstruction/Samples/xs/TestMesh/lossCorner/Scene_L0_B4_R940_dense_sample.ply"))
//    {
//        std::cout << "invalid load mesh" << std::endl;
//        return 0;
//    }

//    glutInit(&argc,argv);
//    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
//    glutInitWindowPosition(100, 100);
//    glutInitWindowSize(1024, 768);
//    glutCreateWindow("Mesh Viewer");

//    initGL(); //load mesh

//    glutKeyboardFunc(&myKeyboard);
//    glutSpecialFunc(&mySpecial);
//    glutReshapeFunc(&myReshape);
//    glutDisplayFunc(&myDisplay);
//    glutMainLoop(); //loop window here


//    return 0;
//}
