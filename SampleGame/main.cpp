#include"D3DApp.h"

D3DApp g_gameApp;//��Ϸ��Ӧ�ó������
///////////////////������///////////////////////////////////
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{

	if (g_gameApp.init(hInstance, (WCHAR*)"D3DApp", 1000, 600, true))//��ʼ����Ϸ��
	{
		g_gameApp.mainLoop();//��Ϸѭ��
		//g_gameApp.unregisterClass();
	}
	return 0;
}