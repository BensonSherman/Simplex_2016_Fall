#include "MyMesh.h"
void MyMesh::Init(void)
{
	m_bBinded = false;
	m_uVertexCount = 0;

	m_VAO = 0;
	m_VBO = 0;

	m_pShaderMngr = ShaderManager::GetInstance();
}
void MyMesh::Release(void)
{
	m_pShaderMngr = nullptr;

	if (m_VBO > 0)
		glDeleteBuffers(1, &m_VBO);

	if (m_VAO > 0)
		glDeleteVertexArrays(1, &m_VAO);

	m_lVertex.clear();
	m_lVertexPos.clear();
	m_lVertexCol.clear();
}
MyMesh::MyMesh()
{
	Init();
}
MyMesh::~MyMesh() { Release(); }
MyMesh::MyMesh(MyMesh& other)
{
	m_bBinded = other.m_bBinded;

	m_pShaderMngr = other.m_pShaderMngr;

	m_uVertexCount = other.m_uVertexCount;

	m_VAO = other.m_VAO;
	m_VBO = other.m_VBO;
}
MyMesh& MyMesh::operator=(MyMesh& other)
{
	if (this != &other)
	{
		Release();
		Init();
		MyMesh temp(other);
		Swap(temp);
	}
	return *this;
}
void MyMesh::Swap(MyMesh& other)
{
	std::swap(m_bBinded, other.m_bBinded);
	std::swap(m_uVertexCount, other.m_uVertexCount);

	std::swap(m_VAO, other.m_VAO);
	std::swap(m_VBO, other.m_VBO);

	std::swap(m_lVertex, other.m_lVertex);
	std::swap(m_lVertexPos, other.m_lVertexPos);
	std::swap(m_lVertexCol, other.m_lVertexCol);

	std::swap(m_pShaderMngr, other.m_pShaderMngr);
}
void MyMesh::CompleteMesh(vector3 a_v3Color)
{
	uint uColorCount = m_lVertexCol.size();
	for (uint i = uColorCount; i < m_uVertexCount; ++i)
	{
		m_lVertexCol.push_back(a_v3Color);
	}
}
void MyMesh::AddVertexPosition(vector3 a_v3Input)
{
	m_lVertexPos.push_back(a_v3Input);
	m_uVertexCount = m_lVertexPos.size();
}
void MyMesh::AddVertexColor(vector3 a_v3Input)
{
	m_lVertexCol.push_back(a_v3Input);
}
void MyMesh::CompileOpenGL3X(void)
{
	if (m_bBinded)
		return;

	if (m_uVertexCount == 0)
		return;

	CompleteMesh();

	for (uint i = 0; i < m_uVertexCount; i++)
	{
		//Position
		m_lVertex.push_back(m_lVertexPos[i]);
		//Color
		m_lVertex.push_back(m_lVertexCol[i]);
	}
	glGenVertexArrays(1, &m_VAO);//Generate vertex array object
	glGenBuffers(1, &m_VBO);//Generate Vertex Buffered Object

	glBindVertexArray(m_VAO);//Bind the VAO
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);//Bind the VBO
	glBufferData(GL_ARRAY_BUFFER, m_uVertexCount * 2 * sizeof(vector3), &m_lVertex[0], GL_STATIC_DRAW);//Generate space for the VBO

	// Position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vector3), (GLvoid*)0);

	// Color attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vector3), (GLvoid*)(1 * sizeof(vector3)));

	m_bBinded = true;

	glBindVertexArray(0); // Unbind VAO
}
void MyMesh::Render(matrix4 a_mProjection, matrix4 a_mView, matrix4 a_mModel)
{
	// Use the buffer and shader
	GLuint nShader = m_pShaderMngr->GetShaderID("Basic");
	glUseProgram(nShader); 

	//Bind the VAO of this object
	glBindVertexArray(m_VAO);

	// Get the GPU variables by their name and hook them to CPU variables
	GLuint MVP = glGetUniformLocation(nShader, "MVP");
	GLuint wire = glGetUniformLocation(nShader, "wire");

	//Final Projection of the Camera
	matrix4 m4MVP = a_mProjection * a_mView * a_mModel;
	glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(m4MVP));
	
	//Solid
	glUniform3f(wire, -1.0f, -1.0f, -1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, m_uVertexCount);  

	//Wire
	glUniform3f(wire, 1.0f, 0.0f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_POLYGON_OFFSET_LINE);
	glPolygonOffset(-1.f, -1.f);
	glDrawArrays(GL_TRIANGLES, 0, m_uVertexCount);
	glDisable(GL_POLYGON_OFFSET_LINE);

	glBindVertexArray(0);// Unbind VAO so it does not get in the way of other objects
}
void MyMesh::AddTri(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft)
{
	//C
	//| \
	//A--B
	//This will make the triangle A->B->C 
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);
}
void MyMesh::AddQuad(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft, vector3 a_vTopRight)
{
	//C--D
	//|  |
	//A--B
	//This will make the triangle A->B->C and then the triangle C->B->D
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);

	AddVertexPosition(a_vTopLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopRight);
}
void MyMesh::GenerateCube(float a_fSize, vector3 a_v3Color)
{
	if (a_fSize < 0.01f)
		a_fSize = 0.01f;

	Release();
	Init();

	float fValue = a_fSize * 0.5f;
	//3--2
	//|  |
	//0--1

	vector3 point0(-fValue,-fValue, fValue); //0
	vector3 point1( fValue,-fValue, fValue); //1
	vector3 point2( fValue, fValue, fValue); //2
	vector3 point3(-fValue, fValue, fValue); //3

	vector3 point4(-fValue,-fValue,-fValue); //4
	vector3 point5( fValue,-fValue,-fValue); //5
	vector3 point6( fValue, fValue,-fValue); //6
	vector3 point7(-fValue, fValue,-fValue); //7

	//F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCuboid(vector3 a_v3Dimensions, vector3 a_v3Color)
{
	Release();
	Init();

	vector3 v3Value = a_v3Dimensions * 0.5f;
	//3--2
	//|  |
	//0--1
	vector3 point0(-v3Value.x, -v3Value.y, v3Value.z); //0
	vector3 point1(v3Value.x, -v3Value.y, v3Value.z); //1
	vector3 point2(v3Value.x, v3Value.y, v3Value.z); //2
	vector3 point3(-v3Value.x, v3Value.y, v3Value.z); //3

	vector3 point4(-v3Value.x, -v3Value.y, -v3Value.z); //4
	vector3 point5(v3Value.x, -v3Value.y, -v3Value.z); //5
	vector3 point6(v3Value.x, v3Value.y, -v3Value.z); //6
	vector3 point7(-v3Value.x, v3Value.y, -v3Value.z); //7

	//F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCone(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	//two points that we know will be there and were they will be placed
	vector3 centerBottom = vector3(0.0f, 0.0f, 0.0f);
	vector3 centerTop = vector3(0.0f, a_fHeight, 0.0f);

	//trig to figure out points
	float theta = 2*PI / a_nSubdivisions;

	for (int i = 1; i <= a_nSubdivisions; i++) {
		//x is x, z is y, if you looked down on the circle we're making
		//cos to find x, then multiple by radius, sin to find y, same with radius
		vector3 point1(cos(theta*i)*a_fRadius, 0.0f, sin(theta*i)*a_fRadius);
		vector3 point2(cos(theta*(i + 1))*a_fRadius, 0.0f, sin(theta*(i + 1))*a_fRadius);
		//bottom face triangle
		AddTri(point1, point2, centerBottom);
		//side triangle
		AddTri(point2, point1, centerTop);
	}

	// Replace this with your code
	//GenerateCube(a_fRadius * 2.0f, a_v3Color);
	// -------------------------------

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCylinder(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	//two points that will be there
	vector3 centerBottom = vector3(0.0f, 0.0f, 0.0f);
	vector3 centerTop = vector3(0.0f, a_fHeight, 0.0f);

	//trig to figure out points
	float theta = 2 * PI / a_nSubdivisions;

	for (int i = 1; i <= a_nSubdivisions; i++) {
		//x is x, z is y, if you looked down on the circle we're making
		//cos to find x, then multiple by radius, sin to find y, same with radius
		vector3 point1(cos(theta*i)*a_fRadius, 0.0f, sin(theta*i)*a_fRadius);
		vector3 point2(cos(theta*(i + 1))*a_fRadius, 0.0f, sin(theta*(i + 1))*a_fRadius);
		vector3 point3(cos(theta*i)*a_fRadius, a_fHeight, sin(theta*i)*a_fRadius);
		vector3 point4(cos(theta*(i + 1))*a_fRadius, a_fHeight, sin(theta*(i + 1))*a_fRadius);
		//wall of cylinder
		AddQuad( point3, point4, point1, point2);
		//bottom face triangle
		AddTri(point1, point2, centerBottom);
		//top face triangle
		AddTri(point4, point3, centerTop);
	}


	// Replace this with your code
	//GenerateCube(a_fRadius * 2.0f, a_v3Color);
	// -------------------------------

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateTube(float a_fOuterRadius, float a_fInnerRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fOuterRadius < 0.01f)
		a_fOuterRadius = 0.01f;

	if (a_fInnerRadius < 0.005f)
		a_fInnerRadius = 0.005f;

	if (a_fInnerRadius > a_fOuterRadius)
		std::swap(a_fInnerRadius, a_fOuterRadius);

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	//trig to figure out points
	float theta = 2 * PI / a_nSubdivisions;


	for (int i = 1; i <= a_nSubdivisions; i++) {
		//x is x, z is y, if you looked down on the circle we're making
		//cos to find x, then multiple by radius, sin to find y, same with radius
		//outer points
		vector3 point1(cos(theta*i)*a_fOuterRadius, 0.0f, sin(theta*i)*a_fOuterRadius);
		vector3 point2(cos(theta*(i + 1))*a_fOuterRadius, 0.0f, sin(theta*(i + 1))*a_fOuterRadius);
		vector3 point3(cos(theta*i)*a_fOuterRadius, a_fHeight, sin(theta*i)*a_fOuterRadius);
		vector3 point4(cos(theta*(i + 1))*a_fOuterRadius, a_fHeight, sin(theta*(i + 1))*a_fOuterRadius);
		//inner points
		vector3 point5(cos(theta*i)*a_fInnerRadius, 0.0f, sin(theta*i)*a_fInnerRadius);
		vector3 point6(cos(theta*(i + 1))*a_fInnerRadius, 0.0f, sin(theta*(i + 1))*a_fInnerRadius);
		vector3 point7(cos(theta*i)*a_fInnerRadius, a_fHeight, sin(theta*i)*a_fInnerRadius);
		vector3 point8(cos(theta*(i + 1))*a_fInnerRadius, a_fHeight, sin(theta*(i + 1))*a_fInnerRadius);
		//Outer wall 
		AddQuad(point3, point4, point1, point2);
		//bottom face quad
		AddQuad(point1, point2, point5,point6);
		//top face Quad
		AddQuad(point4, point3, point8, point7);
		//Inner wall
		AddQuad(point5, point6, point7, point8);
	}

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateTorus(float a_fOuterRadius, float a_fInnerRadius, int a_nSubdivisionsA, int a_nSubdivisionsB, vector3 a_v3Color)
{
	if (a_fOuterRadius < 0.01f)
		a_fOuterRadius = 0.01f;

	if (a_fInnerRadius < 0.005f)
		a_fInnerRadius = 0.005f;

	if (a_fInnerRadius > a_fOuterRadius)
		std::swap(a_fInnerRadius, a_fOuterRadius);

	if (a_nSubdivisionsA < 3)
		a_nSubdivisionsA = 3;
	if (a_nSubdivisionsA > 360)
		a_nSubdivisionsA = 360;

	if (a_nSubdivisionsB < 3)
		a_nSubdivisionsB = 3;
	if (a_nSubdivisionsB > 360)
		a_nSubdivisionsB = 360;

	Release();
	Init();

	//average radius
	//will use this to place a circle
	//Think of a circle at each "joint"
	float averageRadius = a_fOuterRadius + a_fInnerRadius / 2;
	//radius of the inner tube, will use to generate points for the cirle
	float tubeRadius = a_fOuterRadius - a_fInnerRadius / 2;

	std::vector<vector3> innerCircle;
	std::vector<std::vector<vector3>> pointGrid;

	//Generating Vertices
	for (int i = 0; i < a_nSubdivisionsA; i++) {
		//Getting location of "Joint Circle"
		float theta = (PI * 2 / a_nSubdivisionsA)*i;
		innerCircle.push_back(vector3(averageRadius*cos(theta), 0, averageRadius*sin(theta)));

		//Generating Vertices based on "Joint Circle"
		std::vector<vector3> temp;
		for (int c = 0; c < a_nSubdivisionsB; c++) {
			float thetaTwo = ((PI * 2 / a_nSubdivisionsB)*c);
			temp.push_back(vector3(innerCircle[i].x + cos(theta)*sin(thetaTwo), cos(thetaTwo), innerCircle[i].z + sin(theta)*sin(thetaTwo)*tubeRadius));
		}
		pointGrid.push_back(temp);
	}
	//Drawing quads

	for (int i = 0; i < a_nSubdivisionsA-1; i++) {
		for (int c = 0; c < a_nSubdivisionsB-1; c++) {
			AddQuad(pointGrid[i][c], pointGrid[i+1][c], pointGrid[i + 1][c + 1], pointGrid[i][c+1]);
		}
		//Last quad
		AddQuad(pointGrid[i][0], pointGrid[i][a_nSubdivisionsB-1], pointGrid[i + 1][a_nSubdivisionsB-1], pointGrid[i + 1][0]);
	}
	
	for (int i = 0; i < a_nSubdivisionsB - 1; i++) {
		AddQuad(pointGrid[a_nSubdivisionsA-1][i], pointGrid[a_nSubdivisionsA-1][i+1], pointGrid[0][i + 1], pointGrid[0][i]);
	}
	//Last quad
	//AddQuad(pointGrid[i][0], pointGrid[i][a_nSubdivisionsB - 1], pointGrid[i + 1][a_nSubdivisionsB - 1], pointGrid[i + 1][0]);


	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateSphere(float a_fRadius, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	//Sets minimum and maximum of subdivisions
	
	//Kept getting five, even after increasing the number to 20 in app class?
	//rigged it, sorry

	if (a_nSubdivisions < 20)
	{
		a_nSubdivisions = 20;
	}
	if (a_nSubdivisions > 20) {
		a_nSubdivisions = 20;
	}
	
	Release();
	Init();

	//Point grid holds all points
	std::vector<std::vector<vector3>> pointGrid;
	//top point
	std::vector<vector3> topPoint;
	topPoint.push_back(vector3(0, a_fRadius* a_fRadius, 0));
	pointGrid.push_back(topPoint);

	

	//looping through, + 2 for top and bottom points.
	for (int i = a_nSubdivisions + 2; i >= 1; i--) {
		//angle formed by y axis to point
		float thetaTwo = (PI / (a_nSubdivisions + 2)) *(i);
		std::vector<vector3> temp;
		for (int c = a_nSubdivisions; c >= 1; c--) {
			float theta = (2 * PI / a_nSubdivisions) *c;


			temp.push_back(vector3(cos(theta)*sin(thetaTwo),  cos(thetaTwo), sin(theta)*sin(thetaTwo))*a_fRadius);
		}
		pointGrid.push_back(temp);
	}

	//bottom point 
	std::vector<vector3> bottomPoint;
	bottomPoint.push_back(vector3(0, -a_fRadius* a_fRadius, 0));
	pointGrid.push_back(bottomPoint);

	//Drawing quads
	for (int i = a_nSubdivisions + 2; i > 1; i--) {
		for (int c = a_nSubdivisions - 1; c > 0; c--) {
			AddQuad(pointGrid[i][c], pointGrid[i][c - 1], pointGrid[i - 1][c], pointGrid[i - 1][c - 1]);

		}
		//Final quads in row
		AddQuad(pointGrid[i][0], pointGrid[i][a_nSubdivisions - 1], pointGrid[i - 1][0], pointGrid[i - 1][a_nSubdivisions - 1]);
	}
	//capping top
	for (int i = a_nSubdivisions - 1; i > 0; i--) {
		AddTri( pointGrid[a_nSubdivisions][i], topPoint[0], pointGrid[a_nSubdivisions][i - 1]);

	}
	//Final Tri 
	AddTri( pointGrid[a_nSubdivisions][0], topPoint[0], pointGrid[a_nSubdivisions][a_nSubdivisions - 1]);

	

	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}