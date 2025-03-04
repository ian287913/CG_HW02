#pragma once

#include <string>
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <Common.h>

typedef OpenMesh::TriMesh_ArrayKernelT<>  TriMesh;

class MyMesh : public TriMesh
{
public:
	MyMesh();
	~MyMesh();

	int FindVertex(MyMesh::Point pointToFind);
	void ClearMesh();
};

class GLMesh
{
public:
	GLMesh();
	~GLMesh();

	bool Init(std::string fileName);
	void Render();
	void LoadTexCoordToShader();

	MyMesh mesh;
	GLuint vao;
	GLuint ebo;
	GLuint vboVertices, vboNormal, vboTexCoord;

private:

	bool LoadModel(std::string fileName);
	void LoadToShader();
};

class MeshObject
{
public:
	MeshObject();
	~MeshObject();

	bool Init(std::string fileName);
	void Render();
	void RenderSelectedFace();
	bool AddSelectedFace(unsigned int faceID);
	void Parameterization(float uvRotateAngle = 0);
	void RenderParameterized();

private:
	GLMesh model;
	std::vector<unsigned int> selectedFace;
	std::vector<unsigned int*> fvIDsPtr;
	std::vector<int> elemCount;

	void CopySelectFace(MyMesh& model);
};

