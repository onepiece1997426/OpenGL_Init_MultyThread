#pragma once
#include <iostream>
//#include <GL\freeglut.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

/// vcg imports
#include <vcg/complex/complex.h>
#include <vcg/complex/algorithms/update/bounding.h>
#include <vcg/complex/algorithms/update/normal.h>
#include <vcg/complex/algorithms/create/platonic.h>
/// wrapper imports
#include <wrap/io_trimesh/import.h>
#include <wrap/gl/trimesh.h>

using namespace vcg;

class CFace;
class CVertex;

struct MyUsedTypes : public UsedTypes<	Use<CVertex>		::AsVertexType,
                                                                                Use<CFace>			::AsFaceType>{};

/// compositing wanted proprieties
class CVertex : public vcg::Vertex< MyUsedTypes, vcg::vertex::Coord3f, vcg::vertex::Normal3f, vcg::vertex::BitFlags>{};
class CFace   : public vcg::Face<  MyUsedTypes, vcg::face::VertexRef, vcg::face::Normal3f, vcg::face::BitFlags > {};
class CMesh   : public vcg::tri::TriMesh< std::vector<CVertex>, std::vector<CFace> > {};


void initGL();
bool SetMesh(std::string fileName);
void UseGLShowMesh(int argc, char** argv);

