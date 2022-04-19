#include <ShowMeshForMultiThread.h>


extern float xRotate;
extern float yRotate;
extern float zRotate;
extern float scale;
extern bool showFace;
extern bool showWire;
extern bool showPoint;
extern float translateX;
extern float translateY;
extern float translateZ;
extern GLuint showFaceList, showWireList, showPointList;
extern GLuint showStrengthList;


//GLuint showMesh1List;  //两个不同的list对应不同Mesh，用于多线程mesh
//GLuint showMesh2List;
//bool showMesh1 = false;
//bool showMesh2 = true;


extern MyMesh mesh;
extern vcg::Point3f avePos;



//每帧调用 画画
void myDisplay_directShow()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glRotatef(xRotate, 1.0f, 0.0f, 0.0f);
    glRotatef(yRotate, 0.0f, 1.0f, 0.0f);
    glRotatef(zRotate, 0.0f, 0.0f, 1.0f);
    glScalef(scale, scale, scale);
    glTranslatef(translateX,translateY,translateZ);

    /*
        直接画在处理速度上不如每帧调用list来得快
        但是当一个显示列表被编译后，它不能被改变
        感觉现实列表是在mainloop之前就被编译好，然后后续就改不了了
    */
    //先写的在最上层渲染
    //比如先写渲染这个面片为颜色A，后写渲染这个面片为颜色B，则最后渲染出的面片为颜色A。

    //DRAW
    //draw line to showWireList
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
    // SHOW FACE
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



    glutSwapBuffers(); //将渲染好的交换出来，将没渲染的换进去
}

void ShowMeshForMultiThread::InitGLWindowForMultyThread(int argc, char** argv)
{
    MyOpenGLShowMesh::InitGLWindow(argc, argv);

    glutDisplayFunc(&myDisplay_directShow);

    glClearColor(0.3, 0.3, 0.3, 0.0);
    glClearDepth(1.0);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    // ------------------- Lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

//    showMesh1List = glGenLists(1);
//    showMesh2List = glGenLists(1);
}


void ShowMeshForMultiThread::UpdateMeshForMultiThread(std::string meshFile)
{
    MyOpenGLShowMesh::LoadMesh(meshFile);

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


    //update list ,but list must be compile before main loop
    /* invalid way */
    /*
    //need change to mesh 2
//    if(showMesh2)
//    {
//        glNewList(showMesh1List,GL_COMPILE);
//        //DRAW
//        //draw line to showWireList
//        glDisable(GL_LIGHTING);
//        glLineWidth(2.0f);
//        glColor3f(0.1f, 0.1f, 0.1f);
//        glBegin(GL_LINES);
//        std::vector<MyMesh::FaceType>& vec_faces = mesh.face;
//        for (auto iter_face = vec_faces.begin();iter_face != vec_faces.end();iter_face++)
//        {
//            for (int i = 0;i<3;i++)
//            {
//                for (int j = i+1;j<3;j++)
//                {
//                    glVertex3f(iter_face->P(i).X(),iter_face->P(i).Y(),iter_face->P(i).Z());
//                    glVertex3f(iter_face->P(j).X(),iter_face->P(j).Y(),iter_face->P(j).Z());
//                }
//            }
//        }
//        glEnd();
//        glEnable(GL_LIGHTING);
//        // SHOW FACE
//        for (auto iter_face = mesh.face.begin();iter_face != mesh.face.end();iter_face++)
//        {
//            glBegin(GL_TRIANGLES);
//            for (int i = 0;i<3;i++)
//            {
//                //glNormal3f(iter_face->N().X(),iter_face->N().Y(),iter_face->N().Z());
//                glVertex3f(iter_face->P(i).X(),iter_face->P(i).Y(),iter_face->P(i).Z());
//                //glVertex3f(mesh.point(*fv_it)[0], mesh.point(*fv_it)[1], mesh.point(*fv_it)[2]);
//            }
//            glEnd();
//        }
//        glEndList();
//        showMesh2 = false;
//        showMesh1 = true;
//        return;
//    }

//    //need change to mesh 1
//    if(showMesh1)
//    {
//        glNewList(showMesh2List,GL_COMPILE);
//        //DRAW
//        //draw line to showWireList
//        glDisable(GL_LIGHTING);
//        glLineWidth(2.0f);
//        glColor3f(0.1f, 0.1f, 0.1f);
//        glBegin(GL_LINES);
//        std::vector<MyMesh::FaceType>& vec_faces = mesh.face;
//        for (auto iter_face = vec_faces.begin();iter_face != vec_faces.end();iter_face++)
//        {
//            for (int i = 0;i<3;i++)
//            {
//                for (int j = i+1;j<3;j++)
//                {
//                    glVertex3f(iter_face->P(i).X(),iter_face->P(i).Y(),iter_face->P(i).Z());
//                    glVertex3f(iter_face->P(j).X(),iter_face->P(j).Y(),iter_face->P(j).Z());
//                }
//            }
//        }
//        glEnd();
//        glEnable(GL_LIGHTING);
//        // SHOW FACE
//        for (auto iter_face = mesh.face.begin();iter_face != mesh.face.end();iter_face++)
//        {
//            glBegin(GL_TRIANGLES);
//            for (int i = 0;i<3;i++)
//            {
//                //glNormal3f(iter_face->N().X(),iter_face->N().Y(),iter_face->N().Z());
//                glVertex3f(iter_face->P(i).X(),iter_face->P(i).Y(),iter_face->P(i).Z());
//                //glVertex3f(mesh.point(*fv_it)[0], mesh.point(*fv_it)[1], mesh.point(*fv_it)[2]);
//            }
//            glEnd();
//        }
//        glEndList();
//        showMesh1 = false;
//        showMesh2 = true;
//        return;
//    }
    */
}
