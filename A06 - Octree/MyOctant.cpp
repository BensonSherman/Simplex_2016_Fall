#include "MyOctant.h"


Simplex::MyOctant::MyOctant(uint a_nMaxLevel, uint a_nIdealEntityCount)
{
	m_uMaxLevel = a_nMaxLevel;
	m_uIdealEntityCount = a_nIdealEntityCount;
}

Simplex::MyOctant::MyOctant(vector3 a_v3Center, float a_fSize)
{
	m_v3Center = a_v3Center;
	m_fSize = a_fSize;
}

Simplex::MyOctant::MyOctant(MyOctant const & other)
{
	m_uMaxLevel = other.m_uMaxLevel;
	m_uIdealEntityCount = other.m_uIdealEntityCount;
	m_v3Center = other.m_v3Center;
	m_fSize = other.m_fSize;
}


Simplex::MyOctant::~MyOctant(void)
{
	delete this;
}

void Simplex::MyOctant::Swap(MyOctant & other)
{
	m_uMaxLevel = other.m_uMaxLevel;
	m_uIdealEntityCount = other.m_uIdealEntityCount;
	m_v3Center = other.m_v3Center;
	m_fSize = other.m_fSize;
}

float Simplex::MyOctant::GetSize(void)
{
	return m_fSize;
}

vector3 Simplex::MyOctant::GetCenterGlobal(void)
{
	return m_v3Center;
}


