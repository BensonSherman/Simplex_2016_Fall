#include "AppClass.h"
matrix4 m_m4Object1 = matrix4(IDENTITY_M4);
void Application::InitVariables(void)
{
	////Change this to your name and email
	//m_sProgrammer = "Alberto Bobadilla - labigm@rit.edu";

	////Alberto needed this at this position for software recording.
	//m_pWindow->setPosition(sf::Vector2i(710, 0));

	//Make MyMesh object
	m_pMesh = new MyMesh();
	m_pMesh->GenerateCube(2.0f, C_BROWN);

	//Make MyMesh object
	m_pMesh1 = new MyMesh();
	m_pMesh1->GenerateCube(1.0f, C_WHITE);

	for (int i = 0; i < 46; i++) {
		MyMesh * tempMesh = new MyMesh();
		tempMesh->GenerateCube(1.0f, C_BLACK);
		boxList[i] = *tempMesh;
	}
	vector3 initTransform[46];
	initTransform[0] = vector3(1.0f, 1.0f, 2.0f);
	initTransform[1] = vector3(1.0f, 2.0f, 2.0f);
	initTransform[2] = vector3(1.0f, 3.0f, 2.0f);
	initTransform[3] = vector3(2.0f, 3.0f, 2.0f);
	initTransform[4] = vector3(2.0f, 4.0f, 2.0f);
	initTransform
	
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();
	matrix4 m4view = m_pCameraMngr->GetViewMatrix();

	

	m_pMesh->Render(m4Projection, m4view, m_m4Object1);
	m_pMesh1->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3( 3.0f, 0.0f, 0.0f)));

	m_m4Object1 = m_m4Object1 * glm::translate(vector3(0.1f, 0, 0))* glm::rotate(matrix4(IDENTITY_M4),0.1f, AXIS_Y);
		
	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();
	
	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();
	
	//draw gui
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	if (m_pMesh != nullptr)
	{
		delete m_pMesh;
		m_pMesh = nullptr;
	}
	SafeDelete(m_pMesh1);
	//release GUI
	ShutdownGUI();
}