#include "EscapeMaze.h"

EscapeMaze::EscapeMaze(LPDIRECT3DDEVICE9 pDevice)
{
	pd3dDevice = pDevice;
	skybox = new SkyBox(pd3dDevice);
	floor = new Quad(2000, 2000, D3DXVECTOR3(0, 0, 0), pd3dDevice);
	wall[0] = new Cube(D3DXVECTOR3(0, 10, -30), D3DXVECTOR3(50, 20, 10), pd3dDevice);
	wall[1] = new Cube(D3DXVECTOR3(-20, 10, 0), D3DXVECTOR3(10, 20, 50), pd3dDevice);
	wall[2] = new Cube(D3DXVECTOR3(20, 10, 0), D3DXVECTOR3(10, 20, 50), pd3dDevice);   
	wall[3] = new Cube(D3DXVECTOR3(40, 10, 30), D3DXVECTOR3(50, 20, 10), pd3dDevice);
	wall[4] = new Cube(D3DXVECTOR3(-40, 10, 30), D3DXVECTOR3(50, 20, 10), pd3dDevice);
	wall[5] = new Cube(D3DXVECTOR3(60, 10, 60), D3DXVECTOR3(10, 20, 70), pd3dDevice);
	wall[6] = new Cube(D3DXVECTOR3(-60, 10, 60), D3DXVECTOR3(10, 20, 70), pd3dDevice);
	wall[7] = new Cube(D3DXVECTOR3(0, 10, 70), D3DXVECTOR3(50, 20, 40), pd3dDevice);
	wall[8] = new Cube(D3DXVECTOR3(40, 10, 80), D3DXVECTOR3(30, 20, 20), pd3dDevice);
	playerTest = new Cube(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(1, 1, 1), pd3dDevice);
}

EscapeMaze::~EscapeMaze()
{
	
}

void EscapeMaze::init()
{
	skybox->init();
	skybox->LoadSkyTextureFromFile(L"res\\Texture\\SkyBox\\frontsnow1.jpg", L"res\\Texture\\SkyBox\\backsnow1.jpg", L"res\\Texture\\SkyBox\\leftsnow1.jpg", L"res\\Texture\\SkyBox\\rightsnow1.jpg", L"res\\Texture\\SkyBox\\topsnow1.jpg");//从文件加载前、后、左、右、顶面5个面的纹理图
	floor->init();
	floor->loadTexture(L"res\\desert.bmp");
	for (int i = 0; i < 9; ++i) {
		wall[i]->init();
		wall[i]->loadTexture(L"res\\wall.bmp");
	}
	//playerTest->init();
}

void EscapeMaze::update()
{
}

void EscapeMaze::render()
{
	skybox->render(false);
	floor->render();	
	for (int i = 0; i < 9; ++i) {
		wall[i]->render();
	}
	//playerTest->render();
}
