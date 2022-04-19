#include "header.h"
#include "MyOpenGL.h"


//global params for camera and world pos
//定义，分配内存，以后DebugShowMesh每一个对象（实例）的创建都不再分配内存
float xRotate = 0.0f;
float yRotate= 0.0f;
float zRotate= 0.0f;
float scale = 1.0f;
bool showFace = true;
bool showWire = true;
bool showPoint = false;
float translateX = 0.0f;
float translateY = 0.0f;
float translateZ = 0.0f;
GLuint showFaceList, showWireList, showPointList;
GLuint showStrengthList;



//global
MyMesh mesh;
vcg::Point3f avePos(0,0,0);



//reshape window
void myReshape(GLint w, GLint h)
{
    glViewport(0, 0, static_cast<GLsizei>(w), static_cast<GLsizei>(h));
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //正交投影
//    if (w > h)
//        glOrtho(-static_cast<GLdouble>(w) / h, static_cast<GLdouble>(w) / h, -1.0, 1.0, -1.0, 1.0);
//    else
//        glOrtho(-1.0, 1.0, -static_cast<GLdouble>(h) / w, static_cast<GLdouble>(h) / w, -1.0, 1.0);

    //摄影投影 LookAt放在reshape中
    gluPerspective(60.0f,GLfloat(w) / GLfloat(h),0.5,600);
    gluLookAt(0.0,0.0, 100, 0.0,0.0,0.0, 0.0f,1.0f,0.0f);

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
    case 'q':
        translateZ -= 1.0;
        break;
    case 'e':
        translateZ += 1.0;
        break;
    default:
        break;
    }
    //std::cout << key << std::endl;
    glutPostRedisplay();
}

//reset model
//update?
void myDisplay()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glRotatef(xRotate, 1.0f, 0.0f, 0.0f);
    glRotatef(yRotate, 0.0f, 1.0f, 0.0f);
    glRotatef(zRotate, 0.0f, 0.0f, 1.0f);
    glScalef(scale, scale, scale);
    glTranslatef(translateX,translateY,translateZ);

    //切换之后，从每个list中拿出渲染的东西显示
    if (showFace)
        glCallList(showFaceList);
    if (showPoint)
        glCallList(showPointList);
    if (showWire)
        glCallList(showWireList);

    glCallList(showStrengthList);

    glutSwapBuffers();
}


//key board movement
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
        scale = scale + 0.01;
        break;
    case GLUT_KEY_PAGE_DOWN:
        if(scale > 0.01)
            scale = scale - 0.01;
        break;
    default:
        break;
    }
    glutPostRedisplay();
}



bool MyOpenGLShowMesh::LoadMesh(string fileName)
{
    string suffix = fileName.substr( fileName.find_last_of('.') + 1);
    int loadMask = 0;
    int err = 0;
    if(suffix == "ply")
        err = vcg::tri::io::ImporterPLY<MyMesh>::Open(mesh,fileName.c_str());
    else if(suffix == "obj")
        err = vcg::tri::io::ImporterOBJ<MyMesh>::Open(mesh,fileName.c_str(),loadMask);
    else
    {
        std::cout << "this program not support mesh file " + suffix << std::endl;
        err = 1;
    }
    myMeshData = &mesh;
    return err == 0;
}

bool MyOpenGLShowMesh::InitGLWindow(int argc, char** argv)
{
    xRotate = 0.0f;
    yRotate = 0.0f;
    zRotate = 0.0f;
    scale = 1.0;
    showFace = true;
    showWire = true;
    showPoint = false;
    translateX = 0.0f;
    translateY = 0.0f;
    translateZ = 0.0f;

    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(1024, 768);
    glutCreateWindow("Mesh Viewer");

    glutKeyboardFunc(&myKeyboard);
    glutSpecialFunc(&mySpecial);
    glutReshapeFunc(&myReshape);
    glutDisplayFunc(&myDisplay);

    //应该还需要设置一下透视投影的视锥
    //-----

    return true;
}

//---MyOpenGLShowMesh-------------
bool MyOpenGLShowMesh::InitGLMesh()
{
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


    UpdateGLMesh();

    return true;
}

void MyOpenGLShowMesh::UpdateGLMesh()
{
    avePos = vcg::Point3f(0.0f,0.0f,0.0f);
    std::vector<MyMesh::VertexType>& vec_vertexes = mesh.vert;
    uint totalCnt = 0;
    vcg::Box3<float> bbox = mesh.bbox;
    for(uint j=0;j<vec_vertexes.size();j++)
    {
        avePos = avePos * (float(totalCnt) / (totalCnt+1)) + vec_vertexes[j].P() / (totalCnt+1);
        totalCnt++;
    }
    this->modeAvePose = avePos;
    //translate mesh
    for(uint j=0;j< vec_vertexes.size();j++)
    {
         vec_vertexes[j].P() -= avePos;
    }


    //DRAW
    //draw line to showWireList
    glNewList(showWireList, GL_COMPILE);
    glDisable(GL_LIGHTING);
    glLineWidth(2.0f);
    glColor3f(0.1f, 0.1f, 0.1f);
    glBegin(GL_LINES);
    std::vector<MyMesh::FaceType>& vec_faces = mesh.face;
    for (auto iter_face = vec_faces.begin();iter_face != vec_faces.end();iter_face++)
    {
        for (int i = 0;i<3;i++)
        {
            for (int j = i+1;j<3;j++)
            {
                glVertex3f(iter_face->P(i).X(),iter_face->P(i).Y(),iter_face->P(i).Z());
                glVertex3f(iter_face->P(j).X(),iter_face->P(j).Y(),iter_face->P(j).Z());
            }
        }
    }
    glEnd();
    glEnable(GL_LIGHTING);
    glEndList();


    // SHOW POINT
    glNewList(showPointList, GL_COMPILE);
    glDisable(GL_LIGHTING);
    glPointSize(1.0f);
    glColor3f(0.8f, 0.8f, 0.8f);
    glBegin(GL_POINTS);
    for (auto iter_vert = mesh.vert.begin(); iter_vert !=  mesh.vert.end();iter_vert++)
    {
        glVertex3f(iter_vert->P().X(),iter_vert->P().Y(),iter_vert->P().Z());
    }
    glEnd();
    glEnable(GL_LIGHTING);
    glEndList();

    // SHOW FACE
    glNewList(showFaceList, GL_COMPILE);
    for (auto iter_face = mesh.face.begin();iter_face != mesh.face.end();iter_face++)
    {
        glBegin(GL_TRIANGLES);
        for (int i = 0;i<3;i++)
        {
            //glNormal3f(iter_face->N().X(),iter_face->N().Y(),iter_face->N().Z());
            glVertex3f(iter_face->P(i).X(),iter_face->P(i).Y(),iter_face->P(i).Z());
            //glVertex3f(mesh.point(*fv_it)[0], mesh.point(*fv_it)[1], mesh.point(*fv_it)[2]);
        }
        glEnd();
    }
    glEndList();
}


void MyOpenGLShowMesh::UpdateMesh(string meshFile)
{
    LoadMesh(meshFile);
    InitGLMesh();
}

void MyOpenGLShowMesh::GLMainLoop()
{
    glutMainLoop(); //loop window here
}

//添加强调的点
void MyOpenGLShowMesh::StrengthPoints(std::vector<vcg::Point3f> &vec_Points)
{
    showStrengthList = glGenLists(1);
    glNewList(showStrengthList, GL_COMPILE);
    glDisable(GL_LIGHTING);
    glPointSize(10.0f);
    glColor3f(0.8f, 0.1f, 0.8f);
    glBegin(GL_POINTS);
    for(auto iter_bound = vec_Points.begin();iter_bound != vec_Points.end(); iter_bound++)
    {
        glVertex3f(iter_bound->X(),iter_bound->Y(),iter_bound->Z());
    }
    glEnd();
    glEnable(GL_LIGHTING);
    glEndList();
}
