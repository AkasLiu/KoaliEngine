//======================================================================
//	Desc: ��Ϸ�� ����Ϸ�߼� ��Ⱦ ���д���		
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
	bool initGame(IDirect3DDevice9 *pDevice, HWND hWnd, HINSTANCE hInst);	//��ʼ����Ϸ
	void render();		//��Ⱦ
	void update();		//����
	void destroy();		//������Դ

public:
	IDirect3DDevice9	*m_pd3dDevice;			//Direct3D�豸����
	HINSTANCE			m_hInstance;			//Ӧ�ó���ʵ������
	HWND				m_hWnd;					//���ھ��

public:
	/*ע�⣺����ָ���Ա�����ڹ��캯���н��и���ֵ��ʼ����*/
	Camera *m_pCamera;
	Cube *m_pCube;
	DInputClass *m_pInput;
	Quad *m_floor;
	//SnowSystem *m_snowSystem;

	EscapeMaze *m_escapeMaze;
};

