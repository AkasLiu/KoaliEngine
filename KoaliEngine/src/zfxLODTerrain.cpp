#include"../include/zfxLODTerrain.h"

#define LOWEST 100.0f
#define HIGHEST 140.0f

// ȫ�ֺ���
// num������һ������0����������log2(num)���������֣�С�����ֽ����ص�
// ���磬log2(512)���� 9, log2(514)��log2(1000)Ҳ������ 9
UINT log2(UINT num);
// �����ٸýڵ���ӽڵ㣬�����ٸýڵ�
//void deleteTreeNode( CZFXLODTrnNode* pNode );

CZFXLODTrnNode::CZFXLODTrnNode()
{
	m_pTLChildNode = NULL;
	m_pBLChildNode = NULL;
	m_pTRChildNode = NULL;
	m_pBRChildNode = NULL;
}

CZFXLODTrnNode::~CZFXLODTrnNode()
{
	d3d::Delete<CZFXLODTrnNode *>(m_pTLChildNode);
	d3d::Delete<CZFXLODTrnNode *>(m_pBLChildNode);
	d3d::Delete<CZFXLODTrnNode *>(m_pTRChildNode);
	d3d::Delete<CZFXLODTrnNode *>(m_pBRChildNode);
}

CZFXLODTrnNode::CZFXLODTrnNode(ZFXNODEPOSSIZE nodePosSize)
{
	m_pParentNode = NULL;
	m_bIsLeaf = true;
	m_bReachAcceptableLevel = false;
	m_bTouched = false;
	m_bIsVisible = false;

	m_pTLChildNode = NULL;
	m_pBLChildNode = NULL;
	m_pTRChildNode = NULL;
	m_pBRChildNode = NULL;

	m_nodePosSize = nodePosSize;
}

void CZFXLODTrnNode::SplitNode()
{
	UINT sizeDivide2 = m_nodePosSize.size >> 1;
	UINT sizeDivide4 = sizeDivide2 >> 1;
	// ���Ͻ��ӽڵ�
	ZFXNODEPOSSIZE nodePosSize( m_nodePosSize.x - sizeDivide4, m_nodePosSize.y - sizeDivide4, sizeDivide2 );
	m_pTLChildNode = new CZFXLODTrnNode( nodePosSize );
	m_pTLChildNode->m_pParentNode = this;
	m_pTLChildNode->m_level = m_level - 1;
	m_pTLChildNode->m_EnumValue = TOPLEFT;

	// ���½��ӽڵ�
	nodePosSize.y += sizeDivide2;
	m_pBLChildNode = new CZFXLODTrnNode( nodePosSize );
	m_pBLChildNode->m_pParentNode = this;
	m_pBLChildNode->m_level = m_level -1;
	m_pBLChildNode->m_EnumValue = BOTTOMLEFT;

	// ���½��ӽڵ�
	nodePosSize.x += sizeDivide2;
	m_pBRChildNode = new CZFXLODTrnNode( nodePosSize );
	m_pBRChildNode->m_pParentNode = this;
	m_pBRChildNode->m_level = m_level - 1;
	m_pBRChildNode->m_EnumValue = BOTTOMRIGHT;

	// ���Ͻ��ӽڵ�
	nodePosSize.y -= sizeDivide2;
	m_pTRChildNode = new CZFXLODTrnNode( nodePosSize );
	m_pTRChildNode->m_pParentNode = this;
	m_pTRChildNode->m_level = m_level -1;
	m_pTRChildNode->m_EnumValue = TOPRIGHT;
	
	// ���ѽڵ�󣬱㲻����Ҷ�ӽڵ�
	m_bIsLeaf = false;
}

BOOL CZFXLODTrnNode::IsSplit()
{
	if( m_level > 0 )
		return TRUE;
	else
		return FALSE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////
// implementation of  CZFXLODTerrain
CZFXLODTerrain::CZFXLODTerrain()
{
	m_pHeadNode = NULL;
	m_numNodeRendered = 0;
	m_elevArray = NULL;

	m_pVB = NULL;
	m_pIB = NULL;
	m_vertexArray = NULL;
	m_indexArray = NULL;
	m_lowTexture = NULL;
	m_highTexture = NULL;
}

CZFXLODTerrain::CZFXLODTerrain( LPCTSTR strFileName )
{
	m_pHeadNode = NULL;
	m_numNodeRendered = 0;
	// ���ļ��ж�ȡ�������ݡ���������( .map�ļ�����float������ʽ��ȡ)
	FILE* pFile = _tfopen(strFileName, L"rb");
	int width, height;
	fread( &width, sizeof(int), 1, pFile );
	fread( &height,sizeof(int), 1, pFile );
	m_width = width;

	// ���ٶ�ά�߳�����,���Զ�ȡ���ݽ����ʼ��
	m_elevArray = new float*[m_width];
	for(UINT i=0;i<m_width;i++)
	{
		m_elevArray[i] = new float[m_width];
		fread( m_elevArray[i], sizeof(float), m_width, pFile );
	}
	fclose(pFile);

	// ������������
	m_tuArray = new float[m_width];
	m_tvArray = new float[m_width];

	//��SetDimesion(...)�����п��ٿռ䣬ֻ��֪������ļ��ο�Ȳ�����ȡ����������ֵ
	m_normalArray = NULL;

	for(UINT i=0;i<m_width;i++)
	{
		m_tvArray[i] = m_tuArray[i] = i * 20.0f / (int)m_width;
	}

	m_pVB = NULL;
	m_pIB = NULL;
	m_vertexArray = NULL;
	m_indexArray = NULL;
	m_lowTexture = NULL;
	m_highTexture = NULL;
}

CZFXLODTerrain::~CZFXLODTerrain()
{
	// ����ͷ��㼰�����е��ӽڵ㣬��������ڴ����
	d3d::Delete<CZFXLODTrnNode*>(m_pHeadNode);//����delete pNode����pNode���������������ͷ����ĸ��ӽڵ�

	// �ͷŸ߳�����
	if( m_elevArray != NULL )
	{
		for(UINT i=0;i<m_width;i++)
		{
			delete [] m_elevArray[i];
		}
		delete [] m_elevArray;
	}
	// �ͷŷ�����������
	if( m_normalArray != NULL )
	{
		for(UINT i=0;i<m_width;i++)
		{
			delete [] m_normalArray[i];
		}
		delete [] m_normalArray;
	}
	// �ͷ���������
	delete [] m_tuArray;
	delete [] m_tvArray;
}

void CZFXLODTerrain::SetDimesion( float width, float height/*���εļ��γߴ�*/,D3DXVECTOR2 centerPt/*���ε��е�λ��*/ )
{
	m_fWidth = width;
	m_fHeight = height;
	m_centerPt = centerPt;

	m_GridWidth = m_fWidth/(m_width - 1);
	m_GridHeight = m_fHeight/(m_width - 1);

	// ���ٶ�ά������������,����ʼ��
	m_normalArray = new D3DXVECTOR3*[m_width];
	for(UINT i=0;i<m_width;i++)
	{
		m_normalArray[i] = new D3DXVECTOR3[m_width];
	}
	// ��ά�������������ʼ�����������߽粿��
	for(UINT i=0;i<m_width - 1;i++)
	{
		for(UINT j=0;j<m_width - 1;j++)
		{
			D3DXVECTOR3 vec[2];
			vec[0] = D3DXVECTOR3( -m_GridWidth, m_elevArray[i][j] - m_elevArray[i+1][j], 0 );
			vec[1] = D3DXVECTOR3( 0, m_elevArray[i][j] - m_elevArray[i][j+1], m_GridHeight );
			D3DXVec3Cross( &m_normalArray[i][j], &vec[0], &vec[1] );
			D3DXVec3Normalize( &m_normalArray[i][j], &m_normalArray[i][j] );
		}
	}
	// ���α߽紦 ���������ĳ�ʼ��
	for(UINT i=0;i<m_width;i++)
	{
		m_normalArray[i][m_width-1] = D3DXVECTOR3(0, 1, 0);
		m_normalArray[m_width-1][i] = D3DXVECTOR3(0, 1, 0);
	}
}

HRESULT CZFXLODTerrain::InitShader(LPDIRECT3DDEVICE9 pd3dDevice)
{
	HRESULT hr;
	LPD3DXBUFFER pShaderBuffer;
	// Create vertex shader
	if( FAILED( hr = D3DXCompileShaderFromFileA( "hlsl.vsd", NULL, NULL, "main",
		"vs_2_0", 0/*D3DXSHADER_DEBUG | D3DXSHADER_SKIPOPTIMIZATION*//*Flag*/, &pShaderBuffer, NULL/*Error buffer*/, &m_pVConstTable )))
		return hr;
	if( FAILED( hr = pd3dDevice->CreateVertexShader((DWORD*)pShaderBuffer->GetBufferPointer(), &m_pVShader)))
		return hr;
	// Create pixel shader
	if( FAILED( hr = D3DXCompileShaderFromFileA( "hlsl.psd", NULL, NULL, "main", 
		"ps_2_0", 0/*Flag*/, &pShaderBuffer, NULL/*Error buffer*/, &m_pPConstTable )))
		return hr;
	if( FAILED( hr = pd3dDevice->CreatePixelShader((DWORD*)pShaderBuffer->GetBufferPointer(), &m_pPShader)))
		return hr;
	return hr;
}

void CZFXLODTerrain::InitQuadTree()
{
	m_pHeadNode = new CZFXLODTrnNode( ZFXNODEPOSSIZE( (m_width-1) >> 1, (m_width-1) >> 1, m_width-1 ) );
	m_pHeadNode->m_level = log2( m_width - 1 ) - 1;
	GrowQuadTree(m_pHeadNode);
}


void CZFXLODTerrain::LoadTextureFromFile( LPDIRECT3DDEVICE9 pd3dDevice, LPCTSTR lowTexFile, LPCTSTR highTexFile )
{
	// ��������
	if( FAILED( D3DXCreateTextureFromFile( pd3dDevice, lowTexFile, &m_lowTexture )))
	{
		char strTexFileName[60];
		sprintf( strTexFileName, "..\\%s", lowTexFile );
		if( FAILED( D3DXCreateTextureFromFileA( pd3dDevice, strTexFileName, &m_lowTexture )))
			m_lowTexture = NULL;
	}
	if( FAILED( D3DXCreateTextureFromFile( pd3dDevice, highTexFile, &m_highTexture )))
	{
		char strTexFileName[60];
		sprintf( strTexFileName, "..\\%s", highTexFile );
		if( FAILED( D3DXCreateTextureFromFile( pd3dDevice, highTexFile, &m_highTexture )))
			m_highTexture = NULL;
	}
}

void CZFXLODTerrain::CreateTerrain( LPDIRECT3DDEVICE9 pd3dDevice, Camera* pCamera )
{
	
	m_pCamera = pCamera;
	// �����ڵ㼰�������ӽڵ�����Ի�ԭΪfalse
	ClearTreeFlag(m_pHeadNode);
	
	// �����Ĳ������жϽڵ�Ŀɼ��Ժ�����level�Ƿ�ﵽҪ�󣬲��Խڵ���б��
	m_numNodeRendered = 0;
	m_frustum.Construct( pd3dDevice );
	
	ClimbQuadTree(m_pHeadNode, pd3dDevice);
	// ��̬���ٶ����������������
	UINT vertexArraySize = (m_numNodeRendered<<3) + m_numNodeRendered;  //m_numNodeRendered * 9
	UINT indexArraySize  = (m_numNodeRendered<<4) + (m_numNodeRendered<<3);   //m_numNodeRendered * 24
	m_vertexArray = new ZFXTRNVERTEX[vertexArraySize];
	m_indexArray  = new WORD[indexArraySize];
	// �����Ĳ�����Ѱ������m_bReachAcceptableLevel == true�Ľڵ㣬�������з��������Ľڵ�д���������������������
	m_vertexIndex = 0;
	m_indexIndex  = 0;
	RefineNode(m_pHeadNode);
	// �������㻺�����������,��������ĳЩflag���������
	d3d::Release<LPDIRECT3DVERTEXBUFFER9>(m_pVB);
	d3d::Release<LPDIRECT3DINDEXBUFFER9>(m_pIB);
	// ���� vertex buffer
	if( FAILED( pd3dDevice->CreateVertexBuffer( vertexArraySize * sizeof(ZFXTRNVERTEX),
        D3DUSAGE_DYNAMIC|D3DUSAGE_WRITEONLY/*Usage*/, D3DFVF_ZFXTRNVERTEX, D3DPOOL_SYSTEMMEM/*D3DPOOL*/, &m_pVB, NULL )))
		return;
	void* pVertices;
	if( FAILED(m_pVB->Lock( 0/*offset*/, vertexArraySize * sizeof(ZFXTRNVERTEX), (void**)&pVertices, 
		D3DLOCK_DISCARD|D3DLOCK_NOOVERWRITE/*Flag*/ )))
		return;
	memcpy( pVertices, m_vertexArray, vertexArraySize * sizeof(ZFXTRNVERTEX) );
	m_pVB->Unlock();
	// ���� index buffer
	if( FAILED( pd3dDevice->CreateIndexBuffer( indexArraySize *sizeof(WORD), 
		D3DUSAGE_DYNAMIC|D3DUSAGE_WRITEONLY/*Usage*/, D3DFMT_INDEX16, D3DPOOL_SYSTEMMEM/*D3DPOOL*/, &m_pIB, NULL )))
		return;
	void* pIndices;
	if( FAILED(m_pIB->Lock( 0/*offset*/, indexArraySize * sizeof(WORD), (void**)&pIndices,
		D3DLOCK_DISCARD|D3DLOCK_NOOVERWRITE/*Flag*/ )))
		return;
	memcpy( pIndices, m_indexArray, indexArraySize * sizeof(WORD) );
	m_pIB->Unlock();
	// ���� vertex buffer��ϣ��ͷŶ�������
	delete [] m_vertexArray;
	// ���� index buffer��ϣ��ͷ���������
	delete [] m_indexArray;
}


// 
void  CZFXLODTerrain::RenderTerrain( LPDIRECT3DDEVICE9 pd3dDevice, BOOL bFog )
{
	// pre rendering
	D3DXVECTOR4 lightDirVec(3.0f, 1.0f, 0, 0);

	// set vertex shader constant table
	D3DXMATRIXA16 mWorld, mView, mProj;
	pd3dDevice->GetTransform(D3DTS_WORLD, &mWorld);
	pd3dDevice->GetTransform(D3DTS_VIEW, &mView);
	pd3dDevice->GetTransform(D3DTS_PROJECTION, &mProj);
	D3DXMATRIXA16 mWorldViewProj = mWorld * mView * mProj;
	D3DXMATRIXA16 mWorldView = mWorld * mView;
	m_pVConstTable->SetMatrix(pd3dDevice, "mWorldViewProj", &mWorldViewProj);
	m_pVConstTable->SetMatrix(pd3dDevice, "mWorldView", &mWorldView);
	m_pVConstTable->SetFloat( pd3dDevice, "lowest", LOWEST);
	m_pVConstTable->SetFloat( pd3dDevice, "highest", HIGHEST);
	m_pVConstTable->SetFloat( pd3dDevice, "ambient", 0.5f);
	m_pVConstTable->SetVector(pd3dDevice, "lightDir", &lightDirVec);

	// set pixel shader constant table
	D3DXHANDLE hLowSmpl =  m_pPConstTable->GetConstantByName(NULL, "lowTexSmpl");
	D3DXHANDLE hHighSmpl = m_pPConstTable->GetConstantByName(NULL, "highTexSmpl");
	D3DXCONSTANT_DESC lowSmplDesc, highSmplDesc;
	UINT count;
	m_pPConstTable->GetConstantDesc(hLowSmpl, &lowSmplDesc, &count);
	m_pPConstTable->GetConstantDesc(hHighSmpl,&highSmplDesc,&count);

	// set low texture
	pd3dDevice->SetTexture(lowSmplDesc.RegisterIndex, m_lowTexture);
	pd3dDevice->SetSamplerState(lowSmplDesc.RegisterIndex, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	pd3dDevice->SetSamplerState(lowSmplDesc.RegisterIndex, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pd3dDevice->SetSamplerState(lowSmplDesc.RegisterIndex, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pd3dDevice->SetSamplerState(lowSmplDesc.RegisterIndex, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);
	pd3dDevice->SetSamplerState(lowSmplDesc.RegisterIndex, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);
	// set high texture
	pd3dDevice->SetTexture(highSmplDesc.RegisterIndex, m_highTexture);
	pd3dDevice->SetSamplerState(highSmplDesc.RegisterIndex, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	pd3dDevice->SetSamplerState(highSmplDesc.RegisterIndex, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pd3dDevice->SetSamplerState(highSmplDesc.RegisterIndex, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pd3dDevice->SetSamplerState(highSmplDesc.RegisterIndex, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);
	pd3dDevice->SetSamplerState(highSmplDesc.RegisterIndex, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);

	// set fog
	D3DXVECTOR4 fogColor(0.18f, 0.2f, 0.7f, 1.0f);
	m_pVConstTable->SetFloat(pd3dDevice, "nearFog", 200.0f);
	m_pVConstTable->SetFloat(pd3dDevice, "farFog", 1000.0f);
	m_pPConstTable->SetVector(pd3dDevice, "fogColor", &fogColor);
	if(bFog)
		m_pPConstTable->SetFloat(pd3dDevice, "bFog", 1.0f);
	else
		m_pPConstTable->SetFloat(pd3dDevice, "bFog", 0.0f);

	// rendering
	pd3dDevice->SetFVF( D3DFVF_ZFXTRNVERTEX );
	pd3dDevice->SetStreamSource( 0, m_pVB, 0, sizeof(ZFXTRNVERTEX) );
	pd3dDevice->SetVertexShader(m_pVShader);
	pd3dDevice->SetPixelShader(m_pPShader);
	pd3dDevice->SetIndices( m_pIB );
	pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0/*base index index*/, 0,/*MinVertexIndex*/
		(m_numNodeRendered << 3 ) + m_numNodeRendered/*NumVertices*/,
		0/*StartIndex*/,
		(m_numNodeRendered << 3 )/*PrimitiveCount*/);
	// post rendering
	pd3dDevice->SetVertexShader(NULL);
	pd3dDevice->SetPixelShader(NULL);
}

// ����X�����Y���꣬����õ�ĵ��θ߶�  δ��
float CZFXLODTerrain::GetTerrainElev(float x, float z)
{
	/*float elev;*/
	// ����ö�Ӧ�ڵ� (x, z) ���������
	float xFloat, zFloat;
	xFloat = (x - m_centerPt.x) / m_GridWidth + ((m_width - 1)>>1);
	zFloat = (z - m_centerPt.y) / m_GridHeight + ((m_width - 1)>>1);
	int i, j;
	j = (int)xFloat;  // ȡ������
	i = (int)zFloat;  // ȡ������
	return m_elevArray[i][j];
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// ����Ϊ˽�г�Ա����
// ��ͷ���������һ����ȫ�Ķ�����
void CZFXLODTerrain::GrowQuadTree(CZFXLODTrnNode* pNode)
{
	if(pNode->IsSplit())
	{
		pNode->SplitNode();
		GrowQuadTree( pNode->m_pBLChildNode );
		GrowQuadTree( pNode->m_pTLChildNode );
		GrowQuadTree( pNode->m_pTRChildNode );
		GrowQuadTree( pNode->m_pBRChildNode );
	}
}


// �����Ĳ������жϽڵ�Ŀɼ��Ժ�����level�Ƿ�ﵽҪ�󣬲��Խڵ���б��
void CZFXLODTerrain::ClimbQuadTree( CZFXLODTrnNode* pNode, LPDIRECT3DDEVICE9 pd3dDevice )
{
	pNode->m_bTouched = true;
	if( IsVisible( pNode, pd3dDevice ) )// ���ͷ���ɼ����Ըýڵ���д������򣬺���֮
	{
		pNode->m_bIsVisible = true;
		if( IsReachAcceptableLevel( pNode )||pNode->m_bIsLeaf )
		{// ����ýڵ��level���㹻�ͣ����߸ýڵ���Ҷ�ӽڵ㣬�������±���
			pNode->m_bReachAcceptableLevel = true;
			// ������仰Ҫ��ÿ���ڵ�ֻ�ܱ���չһ��
			m_numNodeRendered ++;
		}
		else
		{// ����ýڵ��level�������ͣ����Ҳ���Ҷ�ӽڵ㣬����������չ����
			// ������չ����֮ǰ��ȷ�������ĸ����ڶ���ĸ��ڵ㱻��չ������
			pNode->m_bReachAcceptableLevel = false;
			CZFXLODTrnNode* pAdjNodeArray[4];
			GetAdjNodeParent4( pAdjNodeArray, pNode );
			for(int i=0;i<4;i++)
			{
				if( pAdjNodeArray[i] != NULL && pAdjNodeArray[i]->m_bTouched == false )
					ClimbQuadTree( pAdjNodeArray[i], pd3dDevice );
			}
			if( false == pNode->m_pBLChildNode->m_bTouched )
				ClimbQuadTree( pNode->m_pBLChildNode, pd3dDevice );
			if( false == pNode->m_pTLChildNode->m_bTouched )
				ClimbQuadTree( pNode->m_pTLChildNode, pd3dDevice );
			if( false == pNode->m_pBRChildNode->m_bTouched )
				ClimbQuadTree( pNode->m_pBRChildNode, pd3dDevice );
			if( false == pNode->m_pTRChildNode->m_bTouched )
				ClimbQuadTree( pNode->m_pTRChildNode, pd3dDevice );
		}
	}
	else
	{
		pNode->m_bIsVisible = false;
		pNode->m_bReachAcceptableLevel = false;
	}
}

// �õ��ڵ���ĸ����ڽڵ�ĸ��ڵ㣬�����ǵ�ָ��洢��һ��������
void CZFXLODTerrain::GetAdjNodeParent4(CZFXLODTrnNode** pAdjNodeArray, CZFXLODTrnNode* pNode)
{
	ZFXNODEPOSSIZE posSize = pNode->GetPosSize();
	// ���Ҹýڵ�������Ǹ��ڵ�ĸ��ڵ�
	pAdjNodeArray[0] = FindParentNode( posSize.x, posSize.y - posSize.size );
	// ���Ҹýڵ�������Ǹ��ڵ�ĸ��ڵ�
	pAdjNodeArray[1] = FindParentNode( posSize.x, posSize.y + posSize.size );
	// ���Ҹýڵ���ߵ��Ǹ��ڵ�ĸ��ڵ�
	pAdjNodeArray[2] = FindParentNode( posSize.x - posSize.size, posSize.y );
	// ���Ҹýڵ��ұߵ��Ǹ��ڵ�ĸ��ڵ�
	pAdjNodeArray[3] = FindParentNode( posSize.x + posSize.size, posSize.y );
}

// �������λ��Ϊ(x, y)���Ǹ��ڵ�ĸ��ڵ�
CZFXLODTrnNode* CZFXLODTerrain::FindParentNode(UINT x, UINT y)
{
	CZFXLODTrnNode* pNode = m_pHeadNode;
	while(1)
	{
		if( pNode == NULL )
			return NULL;
		ZFXNODEPOSSIZE posSize = pNode->GetPosSize();
		if( posSize.x == x )
			return pNode->m_pParentNode; // �ҵ��ýڵ㣬�������ĸ��ڵ�
		else if( posSize.x < x )
		{
			if( posSize.y < y )
				pNode = pNode->m_pBRChildNode;
			else //posSize.y > y
				pNode = pNode->m_pTRChildNode;
		}
		else //posSize.x > x
		{
			if( posSize.y < y )
				pNode = pNode->m_pBLChildNode;
			else //posSize.y > y
				pNode = pNode->m_pTLChildNode;
		}
	}
}
// �������λ��Ϊ(x, y)���Ǹ��ڵ�
CZFXLODTrnNode* CZFXLODTerrain::FindNode(UINT x, UINT y)
{
	CZFXLODTrnNode* pNode = m_pHeadNode;
	while(1)
	{
		if( pNode == NULL )
			return NULL;
		ZFXNODEPOSSIZE posSize = pNode->GetPosSize();
		if( posSize.x == x )
			return pNode; // �ҵ��ýڵ㣬�������ĸ��ڵ�
		else if( posSize.x < x )
		{
			if( posSize.y < y )
				pNode = pNode->m_pBRChildNode;
			else //posSize.y > y
				pNode = pNode->m_pTRChildNode;
		}
		else //posSize.x > x
		{
			if( posSize.y < y )
				pNode = pNode->m_pBLChildNode;
			else //posSize.y > y
				pNode = pNode->m_pTLChildNode;
		}
	}
}

// �ж�ĳ���ڵ��Ƿ�ɼ�

BOOL CZFXLODTerrain::IsVisible(CZFXLODTrnNode* pNode, LPDIRECT3DDEVICE9 pd3dDevice)
{
	ZFXNODEPOSSIZE posSize = pNode->GetPosSize();
	D3DXVECTOR3 centerPos;
	centerPos.x = ( int(posSize.x - ((m_width - 1)>>1)) ) * m_GridWidth + m_centerPt.x;
	centerPos.y = m_elevArray[posSize.y][posSize.x];
	centerPos.z = ( int(posSize.y - ((m_width -1 )>>1)) ) * m_GridHeight + m_centerPt.y;
	float radius = float((int)(posSize.size) * m_GridWidth)/1.414f;
	return m_frustum.CheckSphere( centerPos, radius );
}

// �ж�ĳ���ڵ��level�Ƿ����㹻��
BOOL CZFXLODTerrain::IsReachAcceptableLevel(CZFXLODTrnNode* pNode)
{
	// ����ȡ�ڵ����ĵľ�������ֵ
	ZFXNODEPOSSIZE posSize = pNode->GetPosSize();
	D3DXVECTOR3 centerPos;
	centerPos.x = ( int(posSize.x - ((m_width - 1)>>1)) ) * m_GridWidth + m_centerPt.x;
	centerPos.y = m_elevArray[posSize.y][posSize.x];
	centerPos.z = ( int(posSize.y - ((m_width -1 )>>1)) ) * m_GridHeight + m_centerPt.y;
	// �ٻ�ȡ�۾��ľ�������ֵ
	D3DXVECTOR3 eyePt( m_pCamera->GetPosX(), m_pCamera->GetPosY(), m_pCamera->GetPosZ() );
	// ��ȡ�۾���ڵ�����֮��ľ��Ծ���
	D3DXVECTOR3 vec3 = centerPos - eyePt;
	float distance = D3DXVec3Length( &vec3 );
	////
	D3DXVECTOR3 lookAtVec;
	m_pCamera->GetLookVec(&lookAtVec);
	D3DXVec3Normalize(&lookAtVec, &lookAtVec);
	D3DXVec3Normalize(&vec3, &vec3);
	float fNum = D3DXVec3Dot(&vec3, &lookAtVec);
	if(fNum < 0)
		fNum = 0;
	fNum = fNum/2 + 0.5f;
	if( distance / fNum > float(posSize.size << 4) )
		return TRUE;
	else
		return FALSE;
}

// �����Ĳ�����Ѱ������m_bReachAcceptableLevel == true�Ľڵ㣬�������з��������Ľڵ�д���������������������
// ÿ���ڵ���8����������ɣ�9���������ݣ�24����������
void CZFXLODTerrain::RefineNode(CZFXLODTrnNode* pNode)
{
	if( pNode->m_bIsVisible )
	{
		if( pNode->m_bReachAcceptableLevel/* || !pNode->m_bIsVisible*//*for debug*/)
		{// �ڵ��ϸ�����㹻���������붥���������������
			// �������鸳ֵ��   0 1 2
			//                  3 4 5 
			//                  6 7 8
			// ���㶥���λ����������������
			ZFXNODEPOSSIZE posSize = pNode->GetPosSize();
			UINT sizeDivide2 = posSize.size >> 1;
			float top = ( int(posSize.y - sizeDivide2 - ((m_width -1 )>>1)) ) * m_GridHeight + m_centerPt.y;
			float bottom = ( int(posSize.y + sizeDivide2 - ((m_width -1 )>>1)) ) * m_GridHeight + m_centerPt.y;
			float left = ( int(posSize.x - sizeDivide2 - ((m_width -1 )>>1)) ) * m_GridWidth + m_centerPt.x;
			float right = ( int(posSize.x + sizeDivide2 - ((m_width -1 )>>1)) ) * m_GridWidth + m_centerPt.x;
			float centerX = ( int(posSize.x - ((m_width -1 )>>1)) ) * m_GridWidth + m_centerPt.x;
			float centerY = ( int(posSize.y - ((m_width -1 )>>1)) ) * m_GridWidth + m_centerPt.y;
			// ���ϽǶ���
			m_vertexArray[m_vertexIndex + 0].position.x = left;
			m_vertexArray[m_vertexIndex + 0].position.z = top;
			m_vertexArray[m_vertexIndex + 0].position.y = m_elevArray[posSize.y - sizeDivide2][posSize.x - sizeDivide2];
			m_vertexArray[m_vertexIndex + 0].normal   = m_normalArray[posSize.y - sizeDivide2][posSize.x - sizeDivide2];
			// for debug
			//m_vertexArray[m_vertexIndex + 0].position.y = 0;
			m_vertexArray[m_vertexIndex + 0].tu = m_tuArray[posSize.x - sizeDivide2];
			m_vertexArray[m_vertexIndex + 0].tv = m_tvArray[posSize.y - sizeDivide2];

			// �ϱ߶���
			// �ж����Ϸ����Ǹ����ڽڵ�� m_bTouched �����Ƿ�Ϊtrue,��Ϊ��������ж�ȡ�߳�ֵ������ȡ���ڶ���߳�ֵ��ƽ��ֵ
			m_vertexArray[m_vertexIndex + 1].position.x = centerX;
			m_vertexArray[m_vertexIndex + 1].position.z = top;
			CZFXLODTrnNode* pAdjNode = FindNode( posSize.x, posSize.y - posSize.size );
			if( pAdjNode != NULL )
			{
				if( pAdjNode->m_bTouched )
				{
					m_vertexArray[m_vertexIndex + 1].position.y = m_elevArray[posSize.y - sizeDivide2][posSize.x];
					m_vertexArray[m_vertexIndex + 1].normal   = m_normalArray[posSize.y - sizeDivide2][posSize.x];
				}
				else
				{
					//m_vertexArray[m_vertexIndex + 1].position.y = m_elevArray[posSize.y - sizeDivide2][posSize.x];
					m_vertexArray[m_vertexIndex + 1].position.y = ( m_elevArray[posSize.y - sizeDivide2][posSize.x - sizeDivide2]
						+ m_elevArray[posSize.y - sizeDivide2][posSize.x + sizeDivide2])/2;
					m_vertexArray[m_vertexIndex + 1].normal = ( m_normalArray[posSize.y - sizeDivide2][posSize.x - sizeDivide2]
						+ m_normalArray[posSize.y - sizeDivide2][posSize.x + sizeDivide2])/2;
				}
			}
			else
			{
				m_vertexArray[m_vertexIndex + 1].position.y = m_elevArray[posSize.y - sizeDivide2][posSize.x];
				m_vertexArray[m_vertexIndex + 1].normal   = m_normalArray[posSize.y - sizeDivide2][posSize.x];
			}
			
			// for debug
			//m_vertexArray[m_vertexIndex + 1].position.y = 0;
			m_vertexArray[m_vertexIndex + 1].tu = m_tuArray[posSize.x];
			m_vertexArray[m_vertexIndex + 1].tv = m_tvArray[posSize.y - sizeDivide2];
			// ���ϽǶ���
			m_vertexArray[m_vertexIndex + 2].position.x = right;
			m_vertexArray[m_vertexIndex + 2].position.z = top;
			m_vertexArray[m_vertexIndex + 2].position.y = m_elevArray[posSize.y - sizeDivide2][posSize.x + sizeDivide2];
			m_vertexArray[m_vertexIndex + 2].normal   = m_normalArray[posSize.y - sizeDivide2][posSize.x + sizeDivide2];
			// for debug
			//m_vertexArray[m_vertexIndex + 2].position.y = 0;
			m_vertexArray[m_vertexIndex + 2].tu = m_tuArray[posSize.x + sizeDivide2];
			m_vertexArray[m_vertexIndex + 2].tv = m_tvArray[posSize.y - sizeDivide2];

			// ��߶���
			// �ж����󷽵��Ǹ����ڽڵ�� m_bTouched �����Ƿ�Ϊtrue,��Ϊ��������ж�ȡ�߳�ֵ������ȡ���ڶ���߳�ֵ��ƽ��ֵ
			m_vertexArray[m_vertexIndex + 3].position.x = left;
			m_vertexArray[m_vertexIndex + 3].position.z = centerY;
			pAdjNode = FindNode( posSize.x - posSize.size, posSize.y );
			if( pAdjNode != NULL )
			{
				if( pAdjNode->m_bTouched )
				{
					m_vertexArray[m_vertexIndex + 3].position.y = m_elevArray[posSize.y][posSize.x - sizeDivide2];
					m_vertexArray[m_vertexIndex + 3].normal   = m_normalArray[posSize.y][posSize.x - sizeDivide2];
				}
				else
				{
					//m_vertexArray[m_vertexIndex + 3].position.y = m_elevArray[posSize.y][posSize.x - sizeDivide2];
					m_vertexArray[m_vertexIndex + 3].position.y = ( m_elevArray[posSize.y - sizeDivide2][posSize.x - sizeDivide2]
						+ m_elevArray[posSize.y + sizeDivide2][posSize.x - sizeDivide2])/2;
					m_vertexArray[m_vertexIndex + 3].normal = ( m_normalArray[posSize.y - sizeDivide2][posSize.x - sizeDivide2]
						+ m_normalArray[posSize.y + sizeDivide2][posSize.x - sizeDivide2])/2;
				}
			}
			else
			{
				m_vertexArray[m_vertexIndex + 3].position.y = m_elevArray[posSize.y][posSize.x - sizeDivide2];
				m_vertexArray[m_vertexIndex + 3].normal   = m_normalArray[posSize.y][posSize.x - sizeDivide2];
			}
			
			// for debug
			//m_vertexArray[m_vertexIndex + 3].position.y = 0;
			m_vertexArray[m_vertexIndex + 3].tu = m_tuArray[posSize.x - sizeDivide2];
			m_vertexArray[m_vertexIndex + 3].tv = m_tvArray[posSize.y];

			// ���Ķ���
			m_vertexArray[m_vertexIndex + 4].position.x = centerX;
			m_vertexArray[m_vertexIndex + 4].position.z = centerY;
			m_vertexArray[m_vertexIndex + 4].position.y = m_elevArray[posSize.y][posSize.x];
			m_vertexArray[m_vertexIndex + 4].normal   = m_normalArray[posSize.y][posSize.x];
			// for debug
			//m_vertexArray[m_vertexIndex + 4].position.y = 0;
			m_vertexArray[m_vertexIndex + 4].tu = m_tuArray[posSize.x];
			m_vertexArray[m_vertexIndex + 4].tv = m_tvArray[posSize.y];

			// �ұ߶���
			// �ж����󷽵��Ǹ����ڽڵ�� m_bTouched �����Ƿ�Ϊtrue,��Ϊ��������ж�ȡ�߳�ֵ������ȡ���ڶ���߳�ֵ��ƽ��ֵ
			m_vertexArray[m_vertexIndex + 5].position.x = right;
			m_vertexArray[m_vertexIndex + 5].position.z = centerY;
			pAdjNode = FindNode( posSize.x + posSize.size, posSize.y );
			if( pAdjNode != NULL )
			{
				if( pAdjNode->m_bTouched )
				{
					m_vertexArray[m_vertexIndex + 5].position.y = m_elevArray[posSize.y][posSize.x + sizeDivide2];
					m_vertexArray[m_vertexIndex + 5].normal   = m_normalArray[posSize.y][posSize.x + sizeDivide2];
				}
				else
				{
					//m_vertexArray[m_vertexIndex + 5].position.y = m_elevArray[posSize.y][posSize.x + sizeDivide2];
					m_vertexArray[m_vertexIndex + 5].position.y = ( m_elevArray[posSize.y - sizeDivide2][posSize.x + sizeDivide2]
						+ m_elevArray[posSize.y + sizeDivide2][posSize.x + sizeDivide2])/2;
					m_vertexArray[m_vertexIndex + 5].normal = ( m_normalArray[posSize.y - sizeDivide2][posSize.x + sizeDivide2]
						+ m_normalArray[posSize.y + sizeDivide2][posSize.x + sizeDivide2])/2;
				}
			}
			else
			{
				m_vertexArray[m_vertexIndex + 5].position.y = m_elevArray[posSize.y][posSize.x + sizeDivide2];
				m_vertexArray[m_vertexIndex + 5].normal = m_normalArray[posSize.y][posSize.x + sizeDivide2];
			}
			
			// for debug
			//m_vertexArray[m_vertexIndex + 5].position.y = 0;
			m_vertexArray[m_vertexIndex + 5].tu = m_tuArray[posSize.x + sizeDivide2];
			m_vertexArray[m_vertexIndex + 5].tv = m_tvArray[posSize.y];
			// ���½Ƕ���
			m_vertexArray[m_vertexIndex + 6].position.x = left;
			m_vertexArray[m_vertexIndex + 6].position.z = bottom;
			m_vertexArray[m_vertexIndex + 6].position.y = m_elevArray[posSize.y + sizeDivide2][posSize.x - sizeDivide2];
			m_vertexArray[m_vertexIndex + 6].normal   = m_normalArray[posSize.y + sizeDivide2][posSize.x - sizeDivide2];
			// for debug
			//m_vertexArray[m_vertexIndex + 6].position.y = 0;
			m_vertexArray[m_vertexIndex + 6].tu = m_tuArray[posSize.x - sizeDivide2];
			m_vertexArray[m_vertexIndex + 6].tv = m_tvArray[posSize.y + sizeDivide2];

			// �±߶���
			// �ж����󷽵��Ǹ����ڽڵ�� m_bTouched �����Ƿ�Ϊtrue,��Ϊ��������ж�ȡ�߳�ֵ������ȡ���ڶ���߳�ֵ��ƽ��ֵ
			m_vertexArray[m_vertexIndex + 7].position.x = centerX;
			m_vertexArray[m_vertexIndex + 7].position.z = bottom;
			pAdjNode = FindNode( posSize.x, posSize.y + posSize.size );
			if( pAdjNode != NULL )
			{
				if( pAdjNode->m_bTouched )
				{
					m_vertexArray[m_vertexIndex + 7].position.y = m_elevArray[posSize.y + sizeDivide2][posSize.x];
					m_vertexArray[m_vertexIndex + 7].normal   = m_normalArray[posSize.y + sizeDivide2][posSize.x];
				}
				else
				{
					//m_vertexArray[m_vertexIndex + 7].position.y = m_elevArray[posSize.y + sizeDivide2][posSize.x];
					m_vertexArray[m_vertexIndex + 7].position.y = ( m_elevArray[posSize.y + sizeDivide2][posSize.x - sizeDivide2]
						+ m_elevArray[posSize.y + sizeDivide2][posSize.x + sizeDivide2])/2;
					m_vertexArray[m_vertexIndex + 7].normal = ( m_normalArray[posSize.y + sizeDivide2][posSize.x - sizeDivide2]
						+ m_normalArray[posSize.y + sizeDivide2][posSize.x + sizeDivide2])/2;
				}
			}
			else
			{
				m_vertexArray[m_vertexIndex + 7].position.y = m_elevArray[posSize.y + sizeDivide2][posSize.x];
				m_vertexArray[m_vertexIndex + 7].normal   = m_normalArray[posSize.y + sizeDivide2][posSize.x];
			}
			
			// for debug
			//m_vertexArray[m_vertexIndex + 7].position.y = 0;
			m_vertexArray[m_vertexIndex + 7].tu = m_tuArray[posSize.x];
			m_vertexArray[m_vertexIndex + 7].tv = m_tvArray[posSize.y + sizeDivide2];
			// ���½Ƕ���
			m_vertexArray[m_vertexIndex + 8].position.x = right;
			m_vertexArray[m_vertexIndex + 8].position.z = bottom;
			m_vertexArray[m_vertexIndex + 8].position.y = m_elevArray[posSize.y + sizeDivide2][posSize.x + sizeDivide2];
			m_vertexArray[m_vertexIndex + 8].normal   = m_normalArray[posSize.y + sizeDivide2][posSize.x + sizeDivide2];
			// for debug
			//m_vertexArray[m_vertexIndex + 8].position.y = 0;
			m_vertexArray[m_vertexIndex + 8].tu = m_tuArray[posSize.x + sizeDivide2];
			m_vertexArray[m_vertexIndex + 8].tv = m_tvArray[posSize.y + sizeDivide2];
			

			//// �������鸳ֵ��   0 1 2
			////                  3 4 5 
			////                  6 7 8
			//// ���㶥��ķ�������
			///*ZFXNODEPOSSIZE posSize = pNode->GetPosSize();
			//UINT sizeDivide2 = posSize.size >> 1;*/   // �ýڵ��λ����Ϣ����ԣ�
			//POINT pos[9];  //pos[9]������0~8���ڵ�����λ����Ϣ
			//// pos���鸳ֵ
			//pos[0].x = posSize.x - sizeDivide2;  pos[0].y = posSize.y - sizeDivide2;
			//pos[1].x = posSize.x;                pos[1].y = posSize.y - sizeDivide2;
			//pos[2].x = posSize.x + sizeDivide2;  pos[2].y = posSize.y - sizeDivide2;
			//pos[3].x = posSize.x - sizeDivide2;  pos[3].y = posSize.y;
			//pos[4].x = posSize.x;                pos[4].y = posSize.y;
			//pos[5].x = posSize.x + sizeDivide2;  pos[5].y = posSize.y;
			//pos[6].x = posSize.x - sizeDivide2;  pos[6].y = posSize.y + sizeDivide2;
			//pos[7].x = posSize.x;                pos[7].y = posSize.y + sizeDivide2;
			//pos[8].x = posSize.x + sizeDivide2;  pos[8].y = posSize.y + sizeDivide2;
			//for(int i=0;i<9;i++)
			//{
			//	D3DXVECTOR3 vec[2];
			//	vec[0].x = -m_GridWidth;
			//	// ��ֹpos[i].x + 1����߽�
			//	if((UINT)(pos[i].x) < m_width - 1)
			//		vec[0].y = m_elevArray[pos[i].y][pos[i].x] - m_elevArray[pos[i].y][pos[i].x + 1];
			//	else
			//		vec[0].y = m_elevArray[pos[i].y][pos[i].x - 1] - m_elevArray[pos[i].y][pos[i].x];
			//	vec[0].z = 0;

			//	vec[1].x = 0;
			//	// ��ֹpos[i].y + 1����߽�
			//	if((UINT)(pos[i].y) < m_width - 1)
			//		vec[1].y = m_elevArray[pos[i].y + 1][pos[i].x] - m_elevArray[pos[i].y][pos[i].x];
			//	else
			//		vec[1].y = m_elevArray[pos[i].y][pos[i].x] - m_elevArray[pos[i].y - 1][pos[i].x];
			//	vec[1].z = m_GridHeight;
			//	D3DXVec3Cross( &m_vertexArray[m_vertexIndex + i].normal, &vec[0], &vec[1] );
			//}


			for(UINT i=0;i<9;i++)
			{
				m_vertexArray[m_vertexIndex+i].color = 0xffffffff;
			}
			// �������鸳ֵ��   0 1 2   ��˳ʱ����ת
			//                  3 4 5 
			//                  6 7 8
			m_indexArray[m_indexIndex + 0]  = m_vertexIndex + 0;
			m_indexArray[m_indexIndex + 1]  = m_vertexIndex + 1;
			m_indexArray[m_indexIndex + 2]  = m_vertexIndex + 4;
			m_indexArray[m_indexIndex + 3]  = m_vertexIndex + 1;
			m_indexArray[m_indexIndex + 4]  = m_vertexIndex + 2;
			m_indexArray[m_indexIndex + 5]  = m_vertexIndex + 4;
			m_indexArray[m_indexIndex + 6]  = m_vertexIndex + 2;
			m_indexArray[m_indexIndex + 7]  = m_vertexIndex + 5;
			m_indexArray[m_indexIndex + 8]  = m_vertexIndex + 4;
			m_indexArray[m_indexIndex + 9]  = m_vertexIndex + 5;
			m_indexArray[m_indexIndex + 10] = m_vertexIndex + 8;
			m_indexArray[m_indexIndex + 11] = m_vertexIndex + 4;
			m_indexArray[m_indexIndex + 12] = m_vertexIndex + 8;
			m_indexArray[m_indexIndex + 13] = m_vertexIndex + 7;
			m_indexArray[m_indexIndex + 14] = m_vertexIndex + 4;
			m_indexArray[m_indexIndex + 15] = m_vertexIndex + 7;
			m_indexArray[m_indexIndex + 16] = m_vertexIndex + 6;
			m_indexArray[m_indexIndex + 17] = m_vertexIndex + 4;
			m_indexArray[m_indexIndex + 18] = m_vertexIndex + 6;
			m_indexArray[m_indexIndex + 19] = m_vertexIndex + 3;
			m_indexArray[m_indexIndex + 20] = m_vertexIndex + 4;
			m_indexArray[m_indexIndex + 21] = m_vertexIndex + 3;
			m_indexArray[m_indexIndex + 22] = m_vertexIndex + 0;
			m_indexArray[m_indexIndex + 23] = m_vertexIndex + 4;

			m_vertexIndex += 9;
			m_indexIndex  += 24;
		}
		else
		{
			RefineNode( pNode->m_pTLChildNode );
			RefineNode( pNode->m_pBLChildNode );
			RefineNode( pNode->m_pTRChildNode );
			RefineNode( pNode->m_pBRChildNode );
		}
	}
}


UINT log2(UINT num)
{
	UINT a = 0;
	while(num != 0)
	{
		num = num >> 1;
		a++;
	}
	return a - 1;
}

