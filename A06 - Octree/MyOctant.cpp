#include "MyOctant.h"
using namespace Simplex;
uint MyOctant::m_uOctantCount; //will store the number of octants instantiated
uint MyOctant::m_uMaxLevel;//will store the maximum level an octant can go to
uint MyOctant::m_uIdealEntityCount; //will tell how many ideal Entities this object will contain
MyOctant::MyOctant(uint a_nMaxLevel, uint a_nIdealEntityCount)
{
	m_uMaxLevel = a_nMaxLevel;
	m_uIdealEntityCount = a_nIdealEntityCount;
	m_pEntityMngr = EntityManager::GetInstance();
	m_pMeshMngr = MeshManager::GetInstance();
	
	Init();
	
}

MyOctant::MyOctant(vector3 a_v3Center, float a_fSize)
{
	m_v3Center = a_v3Center;
	m_fSize = a_fSize;
	m_pEntityMngr = EntityManager::GetInstance();
	m_pMeshMngr = MeshManager::GetInstance();
	Init();
}

MyOctant::MyOctant(MyOctant const & other)
{
	m_uMaxLevel = other.m_uMaxLevel;
	m_uIdealEntityCount = other.m_uIdealEntityCount;
	m_v3Center = other.m_v3Center;
	m_fSize = other.m_fSize;
	m_pEntityMngr = EntityManager::GetInstance();
	m_pMeshMngr = MeshManager::GetInstance();
	

}






MyOctant::~MyOctant(void)
{
	delete this;
}

void MyOctant::Swap(MyOctant & other)
{
	m_uMaxLevel = other.m_uMaxLevel;
	m_uIdealEntityCount = other.m_uIdealEntityCount;
	m_v3Center = other.m_v3Center;
	m_fSize = other.m_fSize;
}

float MyOctant::GetSize(void)
{
	return m_fSize;
}

vector3 MyOctant::GetCenterGlobal(void)
{
	return m_v3Center;
}

vector3 MyOctant::GetMinGlobal(void)
{
	return m_v3Min;
}

vector3 MyOctant::GetMaxGlobal(void)
{
	return m_v3Max;
}

bool MyOctant::IsColliding(uint a_uRBIndex)
{
	
	if (m_pEntityMngr->GetRigidBody(a_uRBIndex)->IsColliding(m_pRigidBody)) return true;
	
	return false;
}

void Simplex::MyOctant::Display(Simplex::uint a_nIndex, Simplex::vector3 a_v3Color)
{
	m_pMeshMngr->AddWireCubeToRenderList(glm::translate(m_lChild[a_nIndex]->m_v3Center)*glm::length(m_lChild[a_nIndex]->m_v3Max - m_lChild[a_nIndex]->m_v3Center), a_v3Color);
}

void Simplex::MyOctant::Display(vector3 a_v3Color)
{
	m_pMeshMngr->AddWireCubeToRenderList(glm::translate(m_v3Center)*glm::scale(vector3(m_fSize,m_fSize,m_fSize)), a_v3Color);
}

void Simplex::MyOctant::DisplayLeafs(vector3 a_v3Color)
{
	//recursive loop to figure out what's a leaf and what's not
	if (m_uChildren = 8) {
		for (int i = 0; i < 8; i++) {
			DisplayLeafs(C_YELLOW);
		}
	}
	else {
		//this guy is a leaf
		Display(C_YELLOW);
	}
	
}

void Simplex::MyOctant::ClearEntityList(void)
{
	
	
	
}

void Simplex::MyOctant::Subdivide(void)
{
	//making 8 new octants
	m_uChildren = 8;
	

	//getting points inbetween the center and outside points on the square and cutting them in half
	m_pChild[0] = new MyOctant((m_v3Center + m_v3Min)*0.5f, m_fSize / 2);
	for (int i = 0; i < m_EntityList.size; i++) {
		if (m_pChild[0]->IsColliding(m_EntityList[i])) {
			m_pChild[0]->m_EntityList.push_back(m_EntityList[i]);
			m_EntityList.erase(m_EntityList.front+i);
		}
	}
	//everytime we subdivide check to see if we need to subdivide again
	if (m_pChild[0]->ContainsMoreThan(m_uIdealEntityCount)) m_pChild[0]->Subdivide();

	//repeat
	m_pChild[1] = new MyOctant((m_v3Center + vector3(m_v3Max.x, m_v3Min.y, m_v3Min.z))*0.5f, m_fSize / 2);
	for (int i = 0; i < m_EntityList.size; i++) {
		if (m_pChild[1]->IsColliding(m_EntityList[i])) {
			m_pChild[1]->m_EntityList.push_back(m_EntityList[i]);
			m_EntityList.erase(m_EntityList.front + i);
		}
	}
	if (m_pChild[1]->ContainsMoreThan(m_uIdealEntityCount)) m_pChild[0]->Subdivide();

	m_pChild[2] = new MyOctant((m_v3Center + vector3(m_v3Min.x, m_v3Max.y, m_v3Min.z))*0.5f, m_fSize / 2);
	for (int i = 0; i < m_EntityList.size; i++) {
		if (m_pChild[2]->IsColliding(m_EntityList[i])) {
			m_pChild[2]->m_EntityList.push_back(m_EntityList[i]);
			m_EntityList.erase(m_EntityList.front + i);
		}
	}
	if (m_pChild[2]->ContainsMoreThan(m_uIdealEntityCount)) m_pChild[0]->Subdivide();

	m_pChild[3] = new MyOctant((m_v3Center + vector3(m_v3Min.x,m_v3Min.y, m_v3Max.z))*0.5f, m_fSize / 2);
	for (int i = 0; i < m_EntityList.size; i++) {
		if (m_pChild[3]->IsColliding(m_EntityList[i])) {
			m_pChild[3]->m_EntityList.push_back(m_EntityList[i]);
			m_EntityList.erase(m_EntityList.front + i);
		}
	}
	if (m_pChild[3]->ContainsMoreThan(m_uIdealEntityCount)) m_pChild[0]->Subdivide();

	m_pChild[4] = new MyOctant((m_v3Center + vector3(m_v3Min.x, m_v3Max.y, m_v3Max.z))*0.5f, m_fSize / 2);
	for (int i = 0; i < m_EntityList.size; i++) {
		if (m_pChild[4]->IsColliding(m_EntityList[i])) {
			m_pChild[4]->m_EntityList.push_back(m_EntityList[i]);
			m_EntityList.erase(m_EntityList.front + i);
		}
	}
	if (m_pChild[4]->ContainsMoreThan(m_uIdealEntityCount)) m_pChild[0]->Subdivide();

	m_pChild[5] = new MyOctant((m_v3Center + vector3(m_v3Max.x, m_v3Min.y, m_v3Max.z))*0.5f, m_fSize / 2);
	for (int i = 0; i < m_EntityList.size; i++) {
		if (m_pChild[5]->IsColliding(m_EntityList[i])) {
			m_pChild[5]->m_EntityList.push_back(m_EntityList[i]);
			m_EntityList.erase(m_EntityList.front + i);
		}
	}
	if (m_pChild[5]->ContainsMoreThan(m_uIdealEntityCount)) m_pChild[0]->Subdivide();

	m_pChild[6] = new MyOctant((m_v3Center + vector3(m_v3Max.x, m_v3Max.y, m_v3Min.z))*0.5f, m_fSize / 2);
	for (int i = 0; i < m_EntityList.size; i++) {
		if (m_pChild[6]->IsColliding(m_EntityList[i])) {
			m_pChild[6]->m_EntityList.push_back(m_EntityList[i]);
			m_EntityList.erase(m_EntityList.front + i);
		}
	}
	if (m_pChild[6]->ContainsMoreThan(m_uIdealEntityCount)) m_pChild[0]->Subdivide();

	m_pChild[7] = new MyOctant((m_v3Center + m_v3Min)*0.5f, m_fSize / 2);
	for (int i = 0; i < m_EntityList.size; i++) {
		if (m_pChild[7]->IsColliding(m_EntityList[i])) {
			m_pChild[7]->m_EntityList.push_back(m_EntityList[i]);
			m_EntityList.erase(m_EntityList.front + i);
		}
	}
	if (m_pChild[7]->ContainsMoreThan(m_uIdealEntityCount)) m_pChild[0]->Subdivide();
	//adding all of the octants to the 'master list'
	for (int i = 0; i < 8;i++) {
		m_lChild.push_back(m_pChild[i]);
	}
}

MyOctant * Simplex::MyOctant::GetChild(Simplex::uint a_nChild)
{
	return m_pChild[a_nChild];
}

MyOctant * Simplex::MyOctant::GetParent(void)
{
	return m_pParent;
}

bool Simplex::MyOctant::IsLeaf(void)
{
	//if it doesn't have children it's a leaf
	if (m_uChildren == 0) {
		return true;
	}
	
	return false;
	
}

bool Simplex::MyOctant::ContainsMoreThan(Simplex::uint a_nEntities)
{
	//if it has more than the number
	if (m_EntityList.size == a_nEntities) return true;

	return false;
}

void Simplex::MyOctant::KillBranches(void)
{

}

void Simplex::MyOctant::ConstructTree(Simplex::uint a_nMaxLevel)
{
}

void Simplex::MyOctant::AssignIDtoEntity(void)
{
}

Simplex::uint Simplex::MyOctant::GetOctantCount(void)
{
	return Simplex::uint();
}

void Simplex::MyOctant::Release(void)
{
}

void Simplex::MyOctant::Init(void)
{
	//when we call init it means we're making a new octant
	m_uOctantCount++;
	//root node needs this
	if (m_fSize == 0.0f) {
		m_fSize = 68.0f;
	}
	m_v3Min = m_v3Center - vector3(m_fSize, m_fSize, m_fSize);
	m_v3Max = m_v3Center + vector3(m_fSize, m_fSize, m_fSize);
	
	//making rigid bodies for octants to see what's inside
	std::vector<vector3> t_lPoints = std::vector<vector3>();
	t_lPoints.push_back(m_v3Max);
	t_lPoints.push_back(m_v3Min);

	m_pRigidBody = new RigidBody(t_lPoints);
}

void Simplex::MyOctant::ConstructList(void)
{
}
