#include "AppClass.h"
matrix4 m_m4Object1 = matrix4(IDENTITY_M4);
MyMesh boxList[46];
vector3 initTransform[46];
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

	//making the amount of boxes required
	for (int i = 0; i < 46; i++) {
		MyMesh * tempMesh = new MyMesh();
		tempMesh->GenerateCube(1.0f, C_BLACK);
		boxList[i] = *tempMesh;
	}
	//list of starting locations
	
	initTransform[0] = vector3(1.0f, 1.0f, 2.0f);
	initTransform[1] = vector3(1.0f, 2.0f, 2.0f);
	initTransform[2] = vector3(1.0f, 3.0f, 2.0f);
	initTransform[3] = vector3(2.0f, 3.0f, 2.0f);
	initTransform[4] = vector3(2.0f, 4.0f, 2.0f);
	initTransform[5] = vector3(3.0f, 1.0f, 2.0f);
	initTransform[6] = vector3(3.0f, 2.0f, 2.0f);
	initTransform[7] = vector3(3.0f, 3.0f, 2.0f);
	initTransform[8] = vector3(3.0f, 4.0f, 2.0f);
	initTransform[9] = vector3(3.0f, 5.0f, 2.0f);
	initTransform[10] = vector3(3.0f, 7.0f, 2.0f);
	initTransform[11] = vector3(4.0f, 0.0f, 2.0f);
	initTransform[12] = vector3(4.0f, 2.0f, 2.0f);
	initTransform[13] = vector3(4.0f, 3.0f, 2.0f);
	initTransform[14] = vector3(4.0f, 5.0f, 2.0f);
	initTransform[15] = vector3(4.0f, 6.0f, 2.0f);
	initTransform[16] = vector3(5.0f, 0.0f, 2.0f);
	initTransform[17] = vector3(5.0f, 2.0f, 2.0f);
	initTransform[18] = vector3(5.0f, 3.0f, 2.0f);
	initTransform[19] = vector3(5.0f, 4.0f, 2.0f);
	initTransform[20] = vector3(5.0f, 5.0f, 2.0f);
	initTransform[21] = vector3(6.0f, 2.0f, 2.0f);
	initTransform[22] = vector3(6.0f, 3.0f, 2.0f);
	initTransform[23] = vector3(6.0f, 4.0f, 2.0f);
	initTransform[24] = vector3(6.0f, 5.0f, 2.0f);
	initTransform[25] = vector3(7.0f, 0.0f, 2.0f);
	initTransform[26] = vector3(7.0f, 2.0f, 2.0f);
	initTransform[27] = vector3(7.0f, 3.0f, 2.0f);
	initTransform[28] = vector3(7.0f, 4.0f, 2.0f);
	initTransform[29] = vector3(8.0f, 5.0f, 2.0f);
	initTransform[30] = vector3(8.0f, 0.0f, 2.0f);
	initTransform[31] = vector3(8.0f, 2.0f, 2.0f);
	initTransform[32] = vector3(8.0f, 3.0f, 2.0f);
	initTransform[33] = vector3(8.0f, 5.0f, 2.0f);
	initTransform[34] = vector3(8.0f, 6.0f, 2.0f);
	initTransform[35] = vector3(9.0f, 1.0f, 2.0f);
	initTransform[36] = vector3(9.0f, 2.0f, 2.0f);
	initTransform[37] = vector3(9.0f, 3.0f, 2.0f);
	initTransform[38] = vector3(9.0f, 4.0f, 2.0f);
	initTransform[39] = vector3(9.0f, 5.0f, 2.0f);
	initTransform[40] = vector3(9.0f, 7.0f, 2.0f);
	initTransform[41] = vector3(10.0f, 3.0f, 2.0f);
	initTransform[42] = vector3(10.0f, 4.0f, 2.0f);
	initTransform[43] = vector3(11.0f, 1.0f, 2.0f);
	initTransform[44] = vector3(11.0f, 2.0f, 2.0f);
	initTransform[45] = vector3(11.0f, 3.0f, 2.0f);


	
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

	

	//m_pMesh->Render(m4Projection, m4view, m_m4Object1);
	//m_pMesh1->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3( 3.0f, 0.0f, 0.0f)));

	//m_m4Object1 = m_m4Object1 * glm::translate(vector3(0.1f, 0, 0))* glm::rotate(matrix4(IDENTITY_M4),0.1f, AXIS_Y);
		
	for (int i = 0; i < 46; i++) {
		boxList[i].Render(m4Projection, m4view, glm::translate(initTransform[i]));
	}
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