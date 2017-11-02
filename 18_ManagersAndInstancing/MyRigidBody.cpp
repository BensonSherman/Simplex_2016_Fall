#include "MyRigidBody.h"

Simplex::MyRigidBody::MyRigidBody(MyMesh* a_pObject)
{
	m_BS = new Mesh();

	if (a_pObject == nullptr)
		return;

	std::vector <vector3> pointList = a_pObject->GetVertexList();

	if (pointList.size < 1)
		return;

	
	for (uint i = 0; i < pointList.size(); i++) {
		if (pointList[i].x < m_v3Min.x)
			m_v3Min.x = pointList[i].x;
		else if (pointList[i].x > m_v3Max.x)
			m_v3Max.x = pointList[i].x;
		if (pointList[i].y < m_v3Min.y)
			m_v3Min.y = pointList[i].y;
		else if (pointList[i].y > m_v3Max.y)
			m_v3Max.y = pointList[i].y;
		if (pointList[i].z < m_v3Min.z)
			m_v3Min.z = pointList[i].z;
		else if (pointList[i].z > m_v3Max.z)
			m_v3Max.z = pointList[i].z;

	}
	vector3 v3Center = m_v3Min + m_v3Max;
	v3Center /= 2.0f;

	float fRadius = glm::distance(v3Center, m_v3Max);

	m_BB = new Mesh();
	m_BB->GenerateCube(fRadius, C_WHITE);


	m_BS->GenerateIcoSphere(1.0f, 5, C_BLUE);
}

void Simplex::MyRigidBody::Render(MyCamera* a_MyCamera, matrix4 a_m4Model)
{
	

	float * matrix = new float[16];
	memcpy(matrix, glm::value_ptr(IDENTITY_M4), 16 * sizeof(float));
	vector3 v3Size = m_v3Max -m_v3Min;
	matrix4 resize = a_m4Model*glm::scale(IDENTITY_M4, v3Size);
	m_BS->RenderWire(a_MyCamera->GetProjectionMatrix(), a_MyCamera->GetViewMatrix(),matrix, 1);
	m_BB->RenderWire(a_MyCamera->GetProjectionMatrix(), a_MyCamera->GetViewMatrix(), matrix, 1);
	SafeDelete(matrix);
}
