#include"../include/SkyBox.h"

//const DWORD SkyBoxVertex::FVF = D3DFVF_XYZ | D3DFVF_TEX1;

SkyBox::SkyBox(float l, LPDIRECT3DDEVICE9 device)
{
	pd3dDevice = device;
	pVertexBuffer = nullptr;
	for (int i = 0; i < 5; ++i)
	{
		pTexture[i] = nullptr;
	}
	skyBoxLength = l;
	position = D3DXVECTOR3(0, 0, 0);
	D3DXMatrixTranslation(&m_world, position.x, position.y, position.z);
}

SkyBox::SkyBox(LPDIRECT3DDEVICE9 device)
{
	pd3dDevice = device;
	pVertexBuffer = nullptr;
	for (int i = 0; i < 5; ++i)
	{
		pTexture[i] = nullptr;
	}
	skyBoxLength = 1000;
	position = D3DXVECTOR3(0, 0, 0);
	D3DXMatrixTranslation(&m_world, position.x, position.y, position.z);
}

SkyBox::~SkyBox()
{
	for (int i = 0; i < 5; i++)
	{
		pTexture[i]->Release();
	}
	pVertexBuffer->Release();	
}

BOOL SkyBox::init()
{
	pd3dDevice->CreateVertexBuffer(20 * sizeof(SkyBoxVertex), 0, SkyBoxVertex::FVF, D3DPOOL_MANAGED, &pVertexBuffer, 0);

	SkyBoxVertex vertices[] =
	{
		//前面的四个顶点
		{ -skyBoxLength / 2, 0.0f,    skyBoxLength / 2, 0.0f, 1.0f, },
		{ -skyBoxLength / 2, skyBoxLength / 2,   skyBoxLength / 2, 0.0f, 0.0f, },
		{ skyBoxLength / 2, 0.0f,    skyBoxLength / 2, 1.0f, 1.0f, },
		{ skyBoxLength / 2, skyBoxLength / 2,   skyBoxLength / 2, 1.0f, 0.0f, },

		//背面的四个顶点
		{ skyBoxLength / 2, 0.0f,   -skyBoxLength / 2, 0.0f, 1.0f, },
		{ skyBoxLength / 2, skyBoxLength / 2,  -skyBoxLength / 2, 0.0f, 0.0f, },
		{ -skyBoxLength / 2, 0.0f,   -skyBoxLength / 2, 1.0f, 1.0f, },
		{ -skyBoxLength / 2, skyBoxLength / 2,  -skyBoxLength / 2, 1.0f, 0.0f, },

		//左面的四个顶点
		{ -skyBoxLength / 2, 0.0f,   -skyBoxLength / 2, 0.0f, 1.0f, },
		{ -skyBoxLength / 2, skyBoxLength / 2,  -skyBoxLength / 2, 0.0f, 0.0f, },
		{ -skyBoxLength / 2, 0.0f,    skyBoxLength / 2, 1.0f, 1.0f, },
		{ -skyBoxLength / 2, skyBoxLength / 2,   skyBoxLength / 2, 1.0f, 0.0f, },

		//右面的四个顶点
		{ skyBoxLength / 2, 0.0f,   skyBoxLength / 2, 0.0f, 1.0f, },
		{ skyBoxLength / 2, skyBoxLength / 2,  skyBoxLength / 2, 0.0f, 0.0f, },
		{ skyBoxLength / 2, 0.0f,  -skyBoxLength / 2, 1.0f, 1.0f, },
		{ skyBoxLength / 2, skyBoxLength / 2, -skyBoxLength / 2, 1.0f, 0.0f, },

		//上面的四个顶点
		{ skyBoxLength / 2, skyBoxLength / 2, -skyBoxLength / 2, 1.0f, 0.0f, },
		{ skyBoxLength / 2, skyBoxLength / 2,  skyBoxLength / 2, 1.0f, 1.0f, },
		{ -skyBoxLength / 2, skyBoxLength / 2, -skyBoxLength / 2, 0.0f, 0.0f, },
		{ -skyBoxLength / 2, skyBoxLength / 2,  skyBoxLength / 2, 0.0f, 1.0f, },
	};

	//准备填充顶点数据
	void* pVertices;
	//2.加锁
	pVertexBuffer->Lock(0, 0, (void**)&pVertices, 0);
	//3.访问。把结构体中的数据直接拷到顶点缓冲区中
	memcpy(pVertices, vertices, sizeof(vertices));
	//4.解锁
	pVertexBuffer->Unlock();

	return TRUE;
}

//-------------------------------------------------------------------------------------------------
// Name:  SkyBoxClass::LoadSkyTextureFromFile( )
// Desc: 天空盒纹理加载函数
//-------------------------------------------------------------------------------------------------
BOOL SkyBox::LoadSkyTextureFromFile(LPCWSTR pFrontTextureFile, LPCWSTR pBackTextureFile, LPCWSTR pLeftTextureFile, LPCWSTR pRightTextureFile, LPCWSTR pTopTextureFile)
{
	//从文件加载五张纹理
	D3DXCreateTextureFromFile(pd3dDevice, pFrontTextureFile, &pTexture[0]);  //前面
	D3DXCreateTextureFromFile(pd3dDevice, pBackTextureFile, &pTexture[1]);  //后面
	D3DXCreateTextureFromFile(pd3dDevice, pLeftTextureFile, &pTexture[2]);  //左面
	D3DXCreateTextureFromFile(pd3dDevice, pRightTextureFile, &pTexture[3]);  //右面
	D3DXCreateTextureFromFile(pd3dDevice, pTopTextureFile, &pTexture[4]);  //上面	
	return TRUE;
}

void SkyBox::render(BOOL bRenderFrame)
{
	pd3dDevice->SetRenderState(D3DRS_LIGHTING, false);

	pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);  //将纹理颜色混合的第一个参数的颜色值用于输出
	pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);   //纹理颜色混合的第一个参数的值就取纹理颜色值
	pd3dDevice->SetTransform(D3DTS_WORLD, &m_world);  //设置世界矩阵
	pd3dDevice->SetStreamSource(0, pVertexBuffer, 0, sizeof(SkyBoxVertex));    //把包含的几何体信息的顶点缓存和渲染流水线相关联  
	pd3dDevice->SetFVF(SkyBoxVertex::FVF);  //设置FVF灵活顶点格式

	//一个for循环，将5个面绘制出来
	for (int i = 0; i < 5; i++)
	{
		pd3dDevice->SetTexture(0, pTexture[i]);
		pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, 2);
	}

	//对是否渲染线框的处理代码
	if (bRenderFrame)  //如果要渲染出线框的话
	{
		pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME); //把填充模式设为线框填充

																	   //一个for循环，将5个面的线框绘制出来
		for (int i = 0; i < 5; i++)
		{
			pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, 2);	//绘制顶点 
		}

		pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);	//把填充模式调回实体填充
	}

	pd3dDevice->SetRenderState(D3DRS_LIGHTING, true);

}

