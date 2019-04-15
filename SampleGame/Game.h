//======================================================================
//	Desc: 游戏类 对游戏逻辑 渲染 进行处理		
//=======================================================================
#pragma once
#include"../KoaliEngine/include/Camera.h"
#include"../KoaliEngine/include/Cube.h"
#include"../KoaliEngine/include/KoaliText.h"
#include"../KoaliEngine/include/DirectInputClass.h"
#include"../KoaliEngine/include/Light.h"
#include"../KoaliEngine/include/Quad.h"
#include"../KoaliEngine/include/ObjectModel.h"
#include"EscapeMaze.h"

enum GameScene {
	maze = 1,
	terrain = 2,
	specual = 3,
};

class Game
{
public:
	Game();
	~Game();
	bool initGame(IDirect3DDevice9 *pDevice, HWND hWnd, HINSTANCE hInst);	//初始化游戏
	void render();		//渲染
	void update();		//更新
	void destroy();		//销毁资源

public:
	IDirect3DDevice9	*m_pd3dDevice;			//Direct3D设备对象
	HINSTANCE			m_hInstance;			//应用程序实例对象
	HWND				m_hWnd;					//窗口句柄

public:
	/*注意：所有指针成员，需在构造函数中进行赋空值初始化。*/
	Camera *m_pCamera;
	Cube *m_pCube;
	DInputClass *m_pInput;
	Quad *m_floor;
	//SnowSystem *m_snowSystem;

	EscapeMaze *m_escapeMaze;
};

