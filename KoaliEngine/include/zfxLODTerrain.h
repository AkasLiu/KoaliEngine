//--------------------------------------------------------------------------------------
// �ļ�: zfxLODTerrain.h�� ���ߣ�����ϲ
//
// ��װ�� LOD���νڵ���CZFXLODTrnNode ��LOD������CZFXLODTerrain ��
//
// 
//
// ��Ȩ���������У�����������ҵ��;
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
	// ���г�Ա
	// ���ڵ�
	CZFXLODTrnNode* m_pParentNode;
	// �ӽڵ�
	CZFXLODTrnNode* m_pTLChildNode;
	CZFXLODTrnNode* m_pBLChildNode;
	CZFXLODTrnNode* m_pTRChildNode;
	CZFXLODTrnNode* m_pBRChildNode;
protected:
	ZFXNODEPOSSIZE m_nodePosSize;
public:
// ����
	// ���ѽڵ�
	void SplitNode();
	ZFXNODEPOSSIZE GetPosSize() { return m_nodePosSize; }
	
// ����
public:
	BOOL IsSplit();
	LODNodeEnum m_EnumValue;
	UINT  m_level;  // m_levelԽ�ͣ�LOD����Խ�ߣ��ڵ�ϸ��Խ�ã�ͷ�ڵ��m_level���
	bool m_bIsLeaf; // �����ýڵ��Ƿ�Ҷ�ӽڵ㣬��ʼ��Ϊtrue
	bool m_bReachAcceptableLevel; // �����ýڵ��level�Ƿ����㹻�ͣ���ʼ��Ϊfalse
	bool m_bTouched; // �����Ĳ�����ʱ���Ƿ���ʹ��ýڵ㣬��ʼ��Ϊfalse
	bool m_bIsVisible; // �����ýڵ��Ƿ�ɼ�����ʼ��Ϊfalse
};

class CZFXLODTerrain{
public:
	CZFXLODTerrain( LPCTSTR strFileName );  //height == width, �Ҷ���2^n + 1
	CZFXLODTerrain();
	~CZFXLODTerrain();
public:
// ���з���
	HRESULT InitShader(LPDIRECT3DDEVICE9 pd3dDevice);
	void InitQuadTree();
	void SetDimesion( float width, float height/*���εļ��γߴ�*/, D3DXVECTOR2 centerPt/*���ε��е�λ��*/ );
	void LoadTextureFromFile( LPDIRECT3DDEVICE9 pd3dDevice, LPCTSTR lowTexFile, LPCTSTR highTexFile );
	void CreateTerrain( LPDIRECT3DDEVICE9 pd3dDevice, Camera* pCamera );
	void RenderTerrain( LPDIRECT3DDEVICE9 pd3dDevice, BOOL bFog );
	// ����X�����Y���꣬����õ�ĵ��θ߶�
	float GetTerrainElev( float x, float z );
	UINT GetTriangles() { return m_numNodeRendered << 3; }

protected:
	// ��ͷ���������һ����ȫ�Ķ�����
	void GrowQuadTree(CZFXLODTrnNode* pNode);
	// ���ýڵ㼰�������ӽڵ�����ԣ�m_bReachAcceptableLevel��m_bTouched��m_bIsVisible���޸�Ϊfalse
	void ClearTreeFlag(CZFXLODTrnNode* pNode);
	// �����Ĳ������жϽڵ�Ŀɼ��Ժ�����level�Ƿ�ﵽҪ�󣬲��Խڵ���б��
	void ClimbQuadTree(CZFXLODTrnNode* pNode, LPDIRECT3DDEVICE9 pd3dDevice);
	// �ж�ĳ���ڵ��Ƿ�ɼ�
	BOOL IsVisible(CZFXLODTrnNode* pNode, LPDIRECT3DDEVICE9 pd3dDevice);
	// �ж�ĳ���ڵ��level�Ƿ����㹻��
	BOOL IsReachAcceptableLevel(CZFXLODTrnNode* pNode);
	// �õ��ڵ���ĸ����ڽڵ�ĸ��ڵ㣬�����ǵ�ָ��洢��һ��������
	void GetAdjNodeParent4(CZFXLODTrnNode** pAdjNodeArray, CZFXLODTrnNode* pNode);
	// �������λ��Ϊ(x, y)���Ǹ��ڵ�ĸ��ڵ�
	CZFXLODTrnNode* FindParentNode(UINT x, UINT y);
	// �������λ��Ϊ(x, y)���Ǹ��ڵ�
	CZFXLODTrnNode* FindNode(UINT x, UINT y);
	// �����Ĳ�����Ѱ������m_bReachAcceptableLevel == true�Ľڵ㣬�������з��������Ľڵ�д���������������������
	void RefineNode(CZFXLODTrnNode* pNode);
protected:
	/*UINT m_height;*/
	UINT m_width;
	CZFXLODTrnNode* m_pHeadNode;

	// ���εļ��γߴ缰����λ��
	float m_fWidth;
	float m_fHeight;
	// m_GridWidth = m_fWidth/(m_width - 1);
	// m_GridHeight = m_fHeight/(m_width - 1);  Ϊ�����ظ�����������࣬�����������浽һ����������
	float m_GridHeight; // �����ȣ��ӿ�����ٶ�
	float m_GridWidth;  // ����߶ȣ��ӿ�����ٶ�
	D3DXVECTOR2 m_centerPt;
	// ���Ӳü���
	KoaliFrustum m_frustum;
	// �����
	Camera* m_pCamera;

	// ��ά�߳�����
	float** m_elevArray;
	// ��ά������������
	D3DXVECTOR3 ** m_normalArray;
	// ������������
	float* m_tuArray;
	float* m_tvArray;
	// ��Ҫ��Ⱦ�ýڵ���
	UINT m_numNodeRendered;
	// ��������
	ZFXTRNVERTEX *m_vertexArray;
	// ��������
	WORD *m_indexArray;
	// ��������
	UINT m_vertexIndex;
	// ������������
	UINT m_indexIndex;
	// ���㻺��
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	// ��������
    LPDIRECT3DINDEXBUFFER9 m_pIB;
	// ����
	LPDIRECT3DTEXTURE9 m_lowTexture;
	LPDIRECT3DTEXTURE9 m_highTexture;
	// Shader
	IDirect3DVertexShader9* m_pVShader;
	IDirect3DPixelShader9 * m_pPShader;
	ID3DXConstantTable* m_pVConstTable;
	ID3DXConstantTable* m_pPConstTable;
	// Constant table
};

// ���ýڵ㼰�������ӽڵ�����ԣ�m_bReachAcceptableLevel��m_bTouched��m_bIsVisible���޸�Ϊfalse
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