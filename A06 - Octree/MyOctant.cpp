#include "MyOctant.h"
using namespace Simplex;

MyOctant::MyOctant(uint a_nMaxLevel, uint a_nIdealEntityCount)
{
	m_uMaxLevel = a_nMaxLevel;
	m_uIdealEntityCount = a_nIdealEntityCount;
}

MyOctant::MyOctant(vector3 a_v3Center, float a_fSize)
{
	m_v3Center = a_v3Center;
	m_fSize = a_fSize;
}

MyOctant::MyOctant(MyOctant const & other)
{
	m_uMaxLevel = other.m_uMaxLevel;
	m_uIdealEntityCount = other.m_uIdealEntityCount;
	m_v3Center = other.m_v3Center;
	m_fSize = other.m_fSize;
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




