//======================================================================
//	Desc: windows ����� ��windows ���ڳ�����г�ʼ�� ��
//=======================================================================
#pragma once
#include"Game.h"

class D3DApp
{
public:
	D3DApp();
	~D3DApp();
	virtual bool init(HINSTANCE hInstance,
		WCHAR *caption, int width, int height, bool bFullScreen = false);
	//���ڹ���
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	void registerClass();			//ע�ᴰ����
	bool createWindow();			//��������
	void mainLoop();				//��Ϣѭ��
	void unregisterClass();			//ע������
	bool initD3D();					//��ʼ��Direct3D�����豸����

public:
	Game		*m_pGame;
	LPDIRECT3DDEVICE9	m_pd3dDevice;	//Direct3D�豸����
	HINSTANCE	m_hInstance;			//Ӧ�ó���ʵ������
	HWND	m_hWnd;						//���ھ��

private:
	int		m_width;					//���ڿ��
	int		m_height;					//���ڸ߶�
	WCHAR	m_szCaption[256];			//���ڱ���
	bool	m_bFullScreen;				//�Ƿ�ȫ��

};

