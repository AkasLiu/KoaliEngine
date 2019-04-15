#include"D3DApp.h"

D3DApp g_gameApp;//游戏类应用程序对象
///////////////////主函数///////////////////////////////////
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{

	if (g_gameApp.init(hInstance, (WCHAR*)"D3DApp", 1000, 600, true))//初始化游戏类
	{
		g_gameApp.mainLoop();//游戏循环
		//g_gameApp.unregisterClass();
	}
	return 0;
}