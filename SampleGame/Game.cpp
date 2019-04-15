#include "Game.h"

Game::Game()
{
	m_pCamera = NULL;
	m_pCube = NULL;
	m_pInput = NULL;
	m_pd3dDevice = NULL;
	m_escapeMaze = NULL;
	//m_snowSystem = NULL;
}


Game::~Game()
{
	d3d::Delete<DInputClass *>(m_pInput);
	d3d::Delete<Camera *>(m_pCamera);
	d3d::Delete<Cube *>(m_pCube);
	//d3d::Delete<SnowSystem *>(m_snowSystem);
}

bool Game::initGame(IDirect3DDevice9 * pDevice, HWND hWnd, HINSTANCE hInst)
{
	m_pd3dDevice = pDevice;
	m_hWnd = hWnd;
	m_hInstance = hInst;

	//����DirectInput��ĳ�ʼ��
	m_pInput = new DInputClass();
	m_pInput->Init(m_hWnd, m_hInstance, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	//�ڴ���ӳ�ʼ������
	srand(timeGetTime());
	m_pCamera = new Camera(Camera::LANDOBJECT);

	m_escapeMaze = new EscapeMaze(m_pd3dDevice);
	m_escapeMaze->init();



	/*BoundingBox *boundingBox = new BoundingBox(D3DXVECTOR3(-50.0f, 0.0f, -50.0f), D3DXVECTOR3(50.0f, 150.0f, 50.0f));
	m_snowSystem = new SnowSystem(boundingBox, 500);
	m_snowSystem->init(m_pd3dDevice, "snowflake.dds");*/

	//test
	Light MyLight;
	MyLight.SetType(D3DLIGHT_DIRECTIONAL);
	MyLight.SetDiffuseColor(255, 255, 255);
	MyLight.Move(0, 5, 0);
	MyLight.Point(0, -1, 0, 0, 0, 0);
	if (FAILED(m_pd3dDevice->SetLight(0, MyLight.GetLight())))
		return false;
	m_pd3dDevice->LightEnable(0, true);
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// Turn off culling, so we see the front and back of the triangle
	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// Turn off D3D lighting, since we are providing our own vertex colors
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, true);



	//maze 
	//1,��պ� 2,���� 3������   4���Թ�  5����ײ���  6�����Xfile��  7�� wall




	return true;
}

// ��Ϸ���� �߼�Ҳ����д������
void Game::update()
{
	//Ҫ��Ҫ�޸�

	static float  currentTime = 0.0f;//��ǰʱ��
	static float  lastTime = 0.0f;//����ʱ��
	static float deltaTime = 0.0f;//ʱ����
	currentTime = (float)timeGetTime();
	deltaTime = (currentTime - lastTime)*0.001f;
	lastTime = currentTime;

	//ʹ��DirectInput���ȡ����
	m_pInput->GetInput();

	//��ײ���ʧ�ܡ���������

	// ��������������ƶ��ӽ�
	if (m_pInput->IsKeyDown(DIK_A))
	{
		m_pCamera->strafe(-20.0f*deltaTime);
	}

	if (m_pInput->IsKeyDown(DIK_D))
	{
		m_pCamera->strafe(20.0f*deltaTime);
	}

	if (m_pInput->IsKeyDown(DIK_W))
	{
		m_pCamera->walk(20.0f*deltaTime);
	}

	if (m_pInput->IsKeyDown(DIK_S))
	{
		m_pCamera->walk(-20.0f*deltaTime);
	}

	if (m_pInput->IsKeyDown(DIK_R))  m_pCamera->fly(100.0f*deltaTime);
	if (m_pInput->IsKeyDown(DIK_F))  m_pCamera->fly(-100.0f*deltaTime);

	//���������������ת�ӽ�
	if (m_pInput->IsKeyDown(DIK_LEFT))   m_pCamera->yaw(-1.0f*deltaTime);
	if (m_pInput->IsKeyDown(DIK_RIGHT))  m_pCamera->yaw(1.0f*deltaTime);
	if (m_pInput->IsKeyDown(DIK_UP))     m_pCamera->pitch(1.0f*deltaTime);
	if (m_pInput->IsKeyDown(DIK_DOWN))   m_pCamera->pitch(-1.0f*deltaTime);

	//ѡ�񳡾�  1:���Թ�  2:����ϵͳ  3:����
	if (m_pInput->IsKeyDown(DIK_1)) 
	{

	}
	if (m_pInput->IsKeyDown(DIK_2))
	{

	}
	if (m_pInput->IsKeyDown(DIK_3))
	{

	}

	//switch update

}

void Game::render()
{
	m_pd3dDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, 0xffffffff, 1.0f, 0);
	m_pd3dDevice->BeginScene();

	/*-------------------------------------------------------------------------------------------------------*/
	/*-------------------------------------------------------------------------------------------------------*/
	//��ʼ����ͼ��
	m_pCamera->set3DCamera(m_pd3dDevice);

	/*KoaliText *text = new KoaliText();
	text->SetColor(d3d::BLUE);
	text->init(m_pd3dDevice);
	text->DrawTheText(L"hello");
	*/

	m_escapeMaze->render();

	/*-------------------------------------------------------------------------------------------------------*/
	/*-------------------------------------------------------------------------------------------------------*/

	m_pd3dDevice->EndScene();
	m_pd3dDevice->Present(0, 0, 0, 0);
}



void Game::destroy()
{
	
}
