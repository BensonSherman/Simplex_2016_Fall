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

	
}

MyOctant::MyOctant(vector3 a_v3Center, float a_fSize)
{
	m_v3Center = a_v3Center;
	m_fSize = a_fSize;
	m_pEntityMngr = EntityManager::GetInstance();
	m_pMeshMngr = MeshManager::GetInstance();
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

MyOctant & Simplex::MyOctant::operator=(MyOctant const & other)
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
	for (int i = 0; i < 5; i++) {
		if (m_pEntityMngr->GetEntity(a_uRBIndex) != m_pEntityMngr->GetEntity(m_EntityList[i])){
			if (m_pEntityMngr->GetRigidBody(a_uRBIndex)->IsColliding(m_pEntityMngr->GetRigidBody(m_EntityList[i]))) return true;
		}
	}
	return false;
}

void Simplex::MyOctant::Display(Simplex::uint a_nIndex, Simplex::vector3 a_v3Color)
{
	m_pMeshMngr->AddWireCubeToRenderList(glm::translate(m_lChild[a_nIndex]->m_v3Center)*glm::length(m_lChild[a_nIndex]->m_v3Max - m_lChild[a_nIndex]->m_v3Center), a_v3Color);
}

void Simplex::MyOctant::Display(vector3 a_v3Color)
{
	m_pMeshMngr->AddWireCubeToRenderList(glm::translate(m_v3Center)*glm::length(m_v3Max - m_v3Center), a_v3Color);
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
	m_pChild[1] = new MyOctant((m_v3Center + vector3(m_v3Max.x, m_v3Min.y, m_v3Min.z))*0.5f, m_fSize / 2);
	m_pChild[2] = new MyOctant((m_v3Center + vector3(m_v3Min.x, m_v3Max.y, m_v3Min.z))*0.5f, m_fSize / 2);
	m_pChild[3] = new MyOctant((m_v3Center + vector3(m_v3Min.x,m_v3Min.y, m_v3Max.z))*0.5f, m_fSize / 2);
	m_pChild[4] = new MyOctant((m_v3Center + vector3(m_v3Min.x, m_v3Max.y, m_v3Max.z))*0.5f, m_fSize / 2);
	m_pChild[5] = new MyOctant((m_v3Center + vector3(m_v3Max.x, m_v3Min.y, m_v3Max.z))*0.5f, m_fSize / 2);
	m_pChild[6] = new MyOctant((m_v3Center + vector3(m_v3Max.x, m_v3Max.y, m_v3Min.z))*0.5f, m_fSize / 2);
	m_pChild[0] = new MyOctant((m_v3Center + m_v3Min)*0.5f, m_fSize / 2);

	//adding all of the octants to the 'master list'
	for (int i = 0; i < 8;i++) {
		m_lChild.push_back(m_pChild[i]);
	}
}

void Simplex::MyOctant::Init(void)
{
	//root node needs this
	if (m_fSize = 0.0f) {
		m_fSize = 20.0f;
	}
	m_v3Min = m_v3Center - vector3(m_fSize, m_fSize, m_fSize);
	m_v3Max = m_v3Center + vector3(m_fSize, m_fSize, m_fSize);
}
