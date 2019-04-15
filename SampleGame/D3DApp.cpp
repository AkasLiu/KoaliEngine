#include "D3DApp.h"

D3DApp::D3DApp()
{
	m_pGame = NULL;
	m_hWnd = NULL;
	m_hInstance = NULL;
	m_pd3dDevice = NULL;
	m_bFullScreen = false;
	m_width = 640;
	m_height = 480;
	wcscpy_s(m_szCaption, L"无标题");
}

D3DApp::~D3DApp()
{
	//m_pGame->destroy();
	d3d::Delete<Game*>(m_pGame);
	d3d::Release<LPDIRECT3DDEVICE9>(m_pd3dDevice);
	UnregisterClass(L"GameEngin", m_hInstance);
}

bool D3DApp::init(HINSTANCE hInstance,
	WCHAR *caption,
	int width,
	int height,
	bool bFullScreen)
{
	m_pGame = new Game();

	m_hInstance = hInstance;
	m_width = width;
	m_height = height;
	m_bFullScreen = bFullScreen;
	if (caption != NULL)
	{
		wcscpy_s(m_szCaption, caption);
	}
	registerClass();
	if (!createWindow())//创建窗口
	{
		return false;
	}
	if (!initD3D())//初始化Direct3D对象及设备
	{
		return false;
	}
	if (!m_pGame->initGame(m_pd3dDevice, m_hWnd, m_hInstance))//初始化其他
	{
		return false;
	}

	//【4】窗口创建四步曲之四：窗口的移动、显示与更新
	MoveWindow(m_hWnd, 250, 80, m_width, m_height, true);		//调整窗口显示时的位置，使窗口左上角位于（250,80）处
	ShowWindow(m_hWnd, SW_SHOW);    //调用ShowWindow函数来显示窗口
	UpdateWindow(m_hWnd);						//对窗口进行更新，就像我们买了新房子要装修一样
}

//-----------------------------------【WndProc( )函数】--------------------------------------
//	描述：窗口过程函数WndProc,对窗口消息进行处理
//------------------------------------------------------------------------------------------------
LRESULT CALLBACK D3DApp::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)						
	{
	case WM_PAINT:						// 若是客户区重绘消息
		//Direct3D_Render(hwnd);                 //调用Direct3D渲染函数
		ValidateRect(hwnd, NULL);		// 更新客户区的显示
		break;									//跳出该switch语句

	case WM_KEYDOWN:					// 若是键盘按下消息
		if (wParam == VK_ESCAPE)    // 如果被按下的键是ESC
			DestroyWindow(hwnd);		// 销毁窗口, 并发送一条WM_DESTROY消息
		break;									//跳出该switch语句

	case WM_DESTROY:					//若是窗口销毁消息
		//Direct3D_CleanUp();			//调用自定义的资源清理函数Game_CleanUp（）进行退出前的资源清理
		PostQuitMessage(0);			//向系统表明有个线程有终止请求。用来响应WM_DESTROY消息
		break;									//跳出该switch语句

	default:										//若上述case条件都不符合，则执行该default语句
		return DefWindowProc(hwnd, message, wParam, lParam);		//调用缺省的窗口过程
	}

	return 0;									//正常退出
}

//-----------------------------------【registerClass( )函数】--------------------------------------
//	描述：对窗口进行注册
//------------------------------------------------------------------------------------------------
void D3DApp::registerClass()
{
	//【1】窗口创建四步曲之一：开始设计一个完整的窗口类
	WNDCLASSEX wndClass = { 0 };							//用WINDCLASSEX定义了一个窗口类
	wndClass.cbSize = sizeof(WNDCLASSEX);			//设置结构体的字节数大小
	wndClass.style = CS_HREDRAW | CS_VREDRAW;		//设置窗口的样式
	wndClass.lpfnWndProc = WndProc;					//设置指向窗口过程函数的指针
	wndClass.cbClsExtra = 0;								//窗口类的附加内存，取0就可以了
	wndClass.cbWndExtra = 0;							//窗口的附加内存，依然取0就行了
	wndClass.hInstance = m_hInstance;						//指定包含窗口过程的程序的实例句柄。
	wndClass.hIcon = (HICON)::LoadImage(NULL, L"icon .ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);  //本地加载自定义ico图标
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);    //指定窗口类的光标句柄。
	wndClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);  //为hbrBackground成员指定一个白色画刷句柄	
	wndClass.lpszMenuName = NULL;						//用一个以空终止的字符串，指定菜单资源的名字。
	wndClass.lpszClassName = L"D3DGame";		//用一个以空终止的字符串，指定窗口类的名字。

	//【2】窗口创建四步曲之二：注册窗口类
	//if (!RegisterClassEx(&wndClass))				//设计完窗口后，需要对窗口类进行注册，这样才能创建该类型的窗口
	//	return -1;

	RegisterClassEx(&wndClass);
}

//-----------------------------------【createWindow( )函数】--------------------------------------
//	描述：对窗口创建
//------------------------------------------------------------------------------------------------
bool D3DApp::createWindow()
{
	//【3】窗口创建四步曲之三：正式创建窗口
	m_hWnd = CreateWindow(L"D3DGame", L"D3D Game Demo",				//喜闻乐见的创建窗口函数CreateWindow
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, m_width,
		m_height, NULL, NULL, m_hInstance, NULL);
	if (NULL == m_hWnd)
	{
		return false;
	}
	else
	{
		return true;
	}
}

//-----------------------------------【mainLoop( )函数】--------------------------------------
//	描述：游戏主循环 【5】消息循环过程
//------------------------------------------------------------------------------------------------
void D3DApp::mainLoop()
{
	MSG msg = { 0 };  //初始化msg
	while (msg.message != WM_QUIT)			//使用while循环
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))   //查看应用程序消息队列，有消息时将队列中的消息派发出去。
		{
			TranslateMessage(&msg);		//将虚拟键消息转换为字符消息
			DispatchMessage(&msg);		//该函数分发一个消息给窗口程序。
		}
		else
		{
			if (GetActiveWindow() == m_hWnd)
			{
				m_pGame->update();
				m_pGame->render();
			}
			else
			{
				Sleep(1);
			}
		}
	}
}

//-----------------------------------【unregisterClass( )函数】--------------------------------------
//	描述：注销窗口
//------------------------------------------------------------------------------------------------
void D3DApp::unregisterClass()
{
	//【6】窗口类的注销
	UnregisterClass(L"D3DGame", m_hInstance);  //程序准备结束，注销窗口类
}

//-----------------------------------【Direct3D_Init( )函数】--------------------------------------
//	描述：Direct3D初始化函数，进行Direct3D的初始化
//------------------------------------------------------------------------------------------------
bool D3DApp::initD3D()
{
	//--------------------------------------------------------------------------------------
	// 【Direct3D初始化四步曲之一，创接口】：创建Direct3D接口对象, 以便用该Direct3D对象创建Direct3D设备对象
	//--------------------------------------------------------------------------------------
	LPDIRECT3D9  pD3D = NULL; //Direct3D接口对象的创建
	if (NULL == (pD3D = Direct3DCreate9(D3D_SDK_VERSION))) //初始化Direct3D接口对象，并进行DirectX版本协商
		return false;

	//--------------------------------------------------------------------------------------
	// 【Direct3D初始化四步曲之二,取信息】：获取硬件设备信息
	//--------------------------------------------------------------------------------------
	D3DCAPS9 caps; int vp = 0;
	if (FAILED(pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps)))
	{
		return false;
	}
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;   //支持硬件顶点运算，我们就采用硬件顶点运算，妥妥的
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING; //不支持硬件顶点运算，无奈只好采用软件顶点运算

	//--------------------------------------------------------------------------------------
	// 【Direct3D初始化四步曲之三，填内容】：填充D3DPRESENT_PARAMETERS结构体
	//--------------------------------------------------------------------------------------
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = m_width;
	d3dpp.BackBufferHeight = m_height;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = m_hWnd;
	d3dpp.Windowed = true;
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.Flags = 0;
	d3dpp.FullScreen_RefreshRateInHz = 0;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	//--------------------------------------------------------------------------------------
	// 【Direct3D初始化四步曲之四，创设备】：创建Direct3D设备接口
	//--------------------------------------------------------------------------------------
	if (FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
		m_hWnd, vp, &d3dpp, &m_pd3dDevice)))
		return false;

	d3d::Release<LPDIRECT3D9>(pD3D);	//LPDIRECT3D9接口对象的使命完成，我们将其释放掉

	//if (!(S_OK == Objects_Init(hwnd))) return E_FAIL;     //调用一次Objects_Init，进行渲染资源的初始化

	return true;
}
