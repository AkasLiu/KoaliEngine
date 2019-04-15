//--------------------------------------------------------------------------------------
// 文件: zfxLODTerrain.h， 作者：曾凡喜
//
// 封装了 LOD地形节点类CZFXLODTrnNode 和LOD地形类CZFXLODTerrain ，
//
// 
//
// 版权归作者所有，不得用于商业用途
//--------------------------------------------------------------------------------------
#ifndef __ZFXLODTERRAIN__H__INCLUDED__
#define __ZFXLODTERRAIN__H__INCLUDED__

#include"../include/KoaliFrustum.h"
#include"D3dUtility.h"
#include"Camera.h"
#include<fstream>

typedef struct ZFXNODEPOSSIZE{
public:
	ZFXNODEPOSSIZE() {};
	ZFXNODEPOSSIZE( UINT _x, UINT _y, UINT _size ) {x = _x; y = _y; size = _size;}
	UINT x;
	UINT y;
	UINT size;
}ZFXNODEPOSSIZE;

typedef struct tagZFXTRNVERTEX{
	D3DXVECTOR3 position; // The position of the vertex.
	D3DXVECTOR3 normal;   // The nomal Vector.
	D3DCOLOR    color;    // The color
	FLOAT       tu, tv;   // The texture coordinates
}ZFXTRNVERTEX;

#define D3DFVF_ZFXTRNVERTEX  D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1

typedef enum { HEADNODE = 0, TOPLEFT, BOTTOMLEFT, BOTTOMRIGHT, TOPRIGHT } LODNodeEnum;

class CZFXLODTrnNode{
public:
	CZFXLODTrnNode();
	CZFXLODTrnNode( ZFXNODEPOSSIZE nodePosSize );
	~CZFXLODTrnNode();

public:
	// 公有成员
	// 父节点
	CZFXLODTrnNode* m_pParentNode;
	// 子节点
	CZFXLODTrnNode* m_pTLChildNode;
	CZFXLODTrnNode* m_pBLChildNode;
	CZFXLODTrnNode* m_pTRChildNode;
	CZFXLODTrnNode* m_pBRChildNode;
protected:
	ZFXNODEPOSSIZE m_nodePosSize;
public:
// 方法
	// 分裂节点
	void SplitNode();
	ZFXNODEPOSSIZE GetPosSize() { return m_nodePosSize; }
	
// 属性
public:
	BOOL IsSplit();
	LODNodeEnum m_EnumValue;
	UINT  m_level;  // m_level越低，LOD级别越高，节点细节越好，头节点的m_level最高
	bool m_bIsLeaf; // 表明该节点是否叶子节点，初始化为true
	bool m_bReachAcceptableLevel; // 表明该节点的level是否已足够低，初始化为false
	bool m_bTouched; // 遍历四叉树的时候是否访问过该节点，初始化为false
	bool m_bIsVisible; // 表明该节点是否可见，初始化为false
};

class CZFXLODTerrain{
public:
	CZFXLODTerrain( LPCTSTR strFileName );  //height == width, 且都是2^n + 1
	CZFXLODTerrain();
	~CZFXLODTerrain();
public:
// 公有方法
	HRESULT InitShader(LPDIRECT3DDEVICE9 pd3dDevice);
	void InitQuadTree();
	void SetDimesion( float width, float height/*地形的几何尺寸*/, D3DXVECTOR2 centerPt/*地形的中点位置*/ );
	void LoadTextureFromFile( LPDIRECT3DDEVICE9 pd3dDevice, LPCTSTR lowTexFile, LPCTSTR highTexFile );
	void CreateTerrain( LPDIRECT3DDEVICE9 pd3dDevice, Camera* pCamera );
	void RenderTerrain( LPDIRECT3DDEVICE9 pd3dDevice, BOOL bFog );
	// 给定X坐标和Y坐标，求出该点的地形高度
	float GetTerrainElev( float x, float z );
	UINT GetTriangles() { return m_numNodeRendered << 3; }

protected:
	// 将头结点生长成一棵完全的二叉树
	void GrowQuadTree(CZFXLODTrnNode* pNode);
	// 将该节点及其所以子节点的属性（m_bReachAcceptableLevel，m_bTouched，m_bIsVisible）修改为false
	void ClearTreeFlag(CZFXLODTrnNode* pNode);
	// 遍历四叉树，判断节点的可见性和它的level是否达到要求，并对节点进行标记
	void ClimbQuadTree(CZFXLODTrnNode* pNode, LPDIRECT3DDEVICE9 pd3dDevice);
	// 判断某个节点是否可见
	BOOL IsVisible(CZFXLODTrnNode* pNode, LPDIRECT3DDEVICE9 pd3dDevice);
	// 判断某个节点的level是否已足够低
	BOOL IsReachAcceptableLevel(CZFXLODTrnNode* pNode);
	// 得到节点的四个相邻节点的父节点，将它们的指针存储在一个数组里
	void GetAdjNodeParent4(CZFXLODTrnNode** pAdjNodeArray, CZFXLODTrnNode* pNode);
	// 查找相对位置为(x, y)的那个节点的父节点
	CZFXLODTrnNode* FindParentNode(UINT x, UINT y);
	// 查找相对位置为(x, y)的那个节点
	CZFXLODTrnNode* FindNode(UINT x, UINT y);
	// 遍历四叉树，寻找树中m_bReachAcceptableLevel == true的节点，并将所有符合条件的节点写进顶点数组和索引数组中
	void RefineNode(CZFXLODTrnNode* pNode);
protected:
	/*UINT m_height;*/
	UINT m_width;
	CZFXLODTrnNode* m_pHeadNode;

	// 地形的几何尺寸及中心位置
	float m_fWidth;
	float m_fHeight;
	// m_GridWidth = m_fWidth/(m_width - 1);
	// m_GridHeight = m_fHeight/(m_width - 1);  为避免重复计算次数过多，将计算结果保存到一个变量当中
	float m_GridHeight; // 网格宽度，加快计算速度
	float m_GridWidth;  // 网格高度，加快计算速度
	D3DXVECTOR2 m_centerPt;
	// 可视裁减体
	KoaliFrustum m_frustum;
	// 摄像机
	Camera* m_pCamera;

	// 二维高程数组
	float** m_elevArray;
	// 二维法线向量数组
	D3DXVECTOR3 ** m_normalArray;
	// 纹理坐标数组
	float* m_tuArray;
	float* m_tvArray;
	// 需要渲染得节点数
	UINT m_numNodeRendered;
	// 顶点数组
	ZFXTRNVERTEX *m_vertexArray;
	// 索引数组
	WORD *m_indexArray;
	// 顶点索引
	UINT m_vertexIndex;
	// 索引缓冲索引
	UINT m_indexIndex;
	// 顶点缓冲
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	// 索引缓冲
    LPDIRECT3DINDEXBUFFER9 m_pIB;
	// 纹理
	LPDIRECT3DTEXTURE9 m_lowTexture;
	LPDIRECT3DTEXTURE9 m_highTexture;
	// Shader
	IDirect3DVertexShader9* m_pVShader;
	IDirect3DPixelShader9 * m_pPShader;
	ID3DXConstantTable* m_pVConstTable;
	ID3DXConstantTable* m_pPConstTable;
	// Constant table
};

// 将该节点及其所以子节点的属性（m_bReachAcceptableLevel，m_bTouched，m_bIsVisible）修改为false
inline void CZFXLODTerrain::ClearTreeFlag(CZFXLODTrnNode* pNode)
{
	
	//pNode->m_bReachAcceptableLevel = false;
	//pNode->m_bIsVisible = false;
	pNode->m_bTouched = false;
	if(!pNode->m_pBLChildNode->m_bIsLeaf)
	{
		ClearTreeFlag( pNode->m_pBLChildNode );
		ClearTreeFlag( pNode->m_pTLChildNode );
		ClearTreeFlag( pNode->m_pTRChildNode );
		ClearTreeFlag( pNode->m_pBRChildNode );
	}
	{
		pNode->m_pBLChildNode->m_bTouched = false;
		pNode->m_pTLChildNode->m_bTouched = false;
		pNode->m_pTRChildNode->m_bTouched = false;
		pNode->m_pBRChildNode->m_bTouched = false;
	}
}
#endif // __ZFXLODTERRAIN__H__INCLUDED__