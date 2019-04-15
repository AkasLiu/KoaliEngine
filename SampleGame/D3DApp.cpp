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
	wcscpy_s(m_szCaption, L"�ޱ���");
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
	if (!createWindow())//��������
	{
		return false;
	}
	if (!initD3D())//��ʼ��Direct3D�����豸
	{
		return false;
	}
	if (!m_pGame->initGame(m_pd3dDevice, m_hWnd, m_hInstance))//��ʼ������
	{
		return false;
	}

	//��4�����ڴ����Ĳ���֮�ģ����ڵ��ƶ�����ʾ�����
	MoveWindow(m_hWnd, 250, 80, m_width, m_height, true);		//����������ʾʱ��λ�ã�ʹ�������Ͻ�λ�ڣ�250,80����
	ShowWindow(m_hWnd, SW_SHOW);    //����ShowWindow��������ʾ����
	UpdateWindow(m_hWnd);						//�Դ��ڽ��и��£��������������·���Ҫװ��һ��
}

//-----------------------------------��WndProc( )������--------------------------------------
//	���������ڹ��̺���WndProc,�Դ�����Ϣ���д���
//------------------------------------------------------------------------------------------------
LRESULT CALLBACK D3DApp::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)						
	{
	case WM_PAINT:						// ���ǿͻ����ػ���Ϣ
		//Direct3D_Render(hwnd);                 //����Direct3D��Ⱦ����
		ValidateRect(hwnd, NULL);		// ���¿ͻ�������ʾ
		break;									//������switch���

	case WM_KEYDOWN:					// ���Ǽ��̰�����Ϣ
		if (wParam == VK_ESCAPE)    // ��������µļ���ESC
			DestroyWindow(hwnd);		// ���ٴ���, ������һ��WM_DESTROY��Ϣ
		break;									//������switch���

	case WM_DESTROY:					//���Ǵ���������Ϣ
		//Direct3D_CleanUp();			//�����Զ������Դ������Game_CleanUp���������˳�ǰ����Դ����
		PostQuitMessage(0);			//��ϵͳ�����и��߳�����ֹ����������ӦWM_DESTROY��Ϣ
		break;									//������switch���

	default:										//������case�����������ϣ���ִ�и�default���
		return DefWindowProc(hwnd, message, wParam, lParam);		//����ȱʡ�Ĵ��ڹ���
	}

	return 0;									//�����˳�
}

//-----------------------------------��registerClass( )������--------------------------------------
//	�������Դ��ڽ���ע��
//------------------------------------------------------------------------------------------------
void D3DApp::registerClass()
{
	//��1�����ڴ����Ĳ���֮һ����ʼ���һ�������Ĵ�����
	WNDCLASSEX wndClass = { 0 };							//��WINDCLASSEX������һ��������
	wndClass.cbSize = sizeof(WNDCLASSEX);			//���ýṹ����ֽ�����С
	wndClass.style = CS_HREDRAW | CS_VREDRAW;		//���ô��ڵ���ʽ
	wndClass.lpfnWndProc = WndProc;					//����ָ�򴰿ڹ��̺�����ָ��
	wndClass.cbClsExtra = 0;								//������ĸ����ڴ棬ȡ0�Ϳ�����
	wndClass.cbWndExtra = 0;							//���ڵĸ����ڴ棬��Ȼȡ0������
	wndClass.hInstance = m_hInstance;						//ָ���������ڹ��̵ĳ����ʵ�������
	wndClass.hIcon = (HICON)::LoadImage(NULL, L"icon .ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);  //���ؼ����Զ���icoͼ��
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);    //ָ��������Ĺ������
	wndClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);  //ΪhbrBackground��Աָ��һ����ɫ��ˢ���	
	wndClass.lpszMenuName = NULL;						//��һ���Կ���ֹ���ַ�����ָ���˵���Դ�����֡�
	wndClass.lpszClassName = L"D3DGame";		//��һ���Կ���ֹ���ַ�����ָ������������֡�

	//��2�����ڴ����Ĳ���֮����ע�ᴰ����
	//if (!RegisterClassEx(&wndClass))				//����괰�ں���Ҫ�Դ��������ע�ᣬ�������ܴ��������͵Ĵ���
	//	return -1;

	RegisterClassEx(&wndClass);
}

//-----------------------------------��createWindow( )������--------------------------------------
//	�������Դ��ڴ���
//------------------------------------------------------------------------------------------------
bool D3DApp::createWindow()
{
	//��3�����ڴ����Ĳ���֮������ʽ��������
	m_hWnd = CreateWindow(L"D3DGame", L"D3D Game Demo",				//ϲ���ּ��Ĵ������ں���CreateWindow
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

//-----------------------------------��mainLoop( )������--------------------------------------
//	��������Ϸ��ѭ�� ��5����Ϣѭ������
//------------------------------------------------------------------------------------------------
void D3DApp::mainLoop()
{
	MSG msg = { 0 };  //��ʼ��msg
	while (msg.message != WM_QUIT)			//ʹ��whileѭ��
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))   //�鿴Ӧ�ó�����Ϣ���У�����Ϣʱ�������е���Ϣ�ɷ���ȥ��
		{
			TranslateMessage(&msg);		//���������Ϣת��Ϊ�ַ���Ϣ
			DispatchMessage(&msg);		//�ú����ַ�һ����Ϣ�����ڳ���
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

//-----------------------------------��unregisterClass( )������--------------------------------------
//	������ע������
//------------------------------------------------------------------------------------------------
void D3DApp::unregisterClass()
{
	//��6���������ע��
	UnregisterClass(L"D3DGame", m_hInstance);  //����׼��������ע��������
}

//-----------------------------------��Direct3D_Init( )������--------------------------------------
//	������Direct3D��ʼ������������Direct3D�ĳ�ʼ��
//------------------------------------------------------------------------------------------------
bool D3DApp::initD3D()
{
	//--------------------------------------------------------------------------------------
	// ��Direct3D��ʼ���Ĳ���֮һ�����ӿڡ�������Direct3D�ӿڶ���, �Ա��ø�Direct3D���󴴽�Direct3D�豸����
	//--------------------------------------------------------------------------------------
	LPDIRECT3D9  pD3D = NULL; //Direct3D�ӿڶ���Ĵ���
	if (NULL == (pD3D = Direct3DCreate9(D3D_SDK_VERSION))) //��ʼ��Direct3D�ӿڶ��󣬲�����DirectX�汾Э��
		return false;

	//--------------------------------------------------------------------------------------
	// ��Direct3D��ʼ���Ĳ���֮��,ȡ��Ϣ������ȡӲ���豸��Ϣ
	//--------------------------------------------------------------------------------------
	D3DCAPS9 caps; int vp = 0;
	if (FAILED(pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps)))
	{
		return false;
	}
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;   //֧��Ӳ���������㣬���ǾͲ���Ӳ���������㣬���׵�
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING; //��֧��Ӳ���������㣬����ֻ�ò��������������

	//--------------------------------------------------------------------------------------
	// ��Direct3D��ʼ���Ĳ���֮���������ݡ������D3DPRESENT_PARAMETERS�ṹ��
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
	// ��Direct3D��ʼ���Ĳ���֮�ģ����豸��������Direct3D�豸�ӿ�
	//--------------------------------------------------------------------------------------
	if (FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
		m_hWnd, vp, &d3dpp, &m_pd3dDevice)))
		return false;

	d3d::Release<LPDIRECT3D9>(pD3D);	//LPDIRECT3D9�ӿڶ����ʹ����ɣ����ǽ����ͷŵ�

	//if (!(S_OK == Objects_Init(hwnd))) return E_FAIL;     //����һ��Objects_Init��������Ⱦ��Դ�ĳ�ʼ��

	return true;
}
