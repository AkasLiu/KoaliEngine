//======================================================================
//	Desc: windows 框架类 对windows 窗口程序进行初始化 等
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
	//窗口过程
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	void registerClass();			//注册窗口类
	bool createWindow();			//创建窗口
	void mainLoop();				//消息循环
	void unregisterClass();			//注销窗口
	bool initD3D();					//初始化Direct3D对象及设备对象

public:
	Game		*m_pGame;
	LPDIRECT3DDEVICE9	m_pd3dDevice;	//Direct3D设备对象
	HINSTANCE	m_hInstance;			//应用程序实例对象
	HWND	m_hWnd;						//窗口句柄

private:
	int		m_width;					//窗口宽度
	int		m_height;					//窗口高度
	WCHAR	m_szCaption[256];			//窗口标题
	bool	m_bFullScreen;				//是否全屏

};

