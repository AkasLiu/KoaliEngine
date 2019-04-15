//--------------------------------------------------------------------------------------
// ��װ�˲ü����� ����ʵ�ֵ�,������,����,����ļ��
//
// ÿ���ӿڸı��ʱ�򣬶���Ҫ���¹���ü��壨Construct�����ٵ��ÿ����Լ�⺯����Check����������
//--------------------------------------------------------------------------------------
#pragma once

#pragma once
#include"D3dUtility.h"

class KoaliFrustum
{
public:
	KoaliFrustum();
	~KoaliFrustum();
public:
	// ����ü��������ƽ�棬ÿ���ӿڸı��ʱ�򣬶���Ҫ���¹���ü��壬�ٵ��ÿ����Լ�⺯��
	void Construct(LPDIRECT3DDEVICE9 pd3dDevice);
	// ����FALSE��ʾ�ڲü���֮�⣨���ɼ�����TRUE��ʾ�ڲü���֮�ڣ��ɼ���
	// ����
	BOOL CheckPoint(D3DXVECTOR3 ptPos);
	// ��������
	BOOL CheckCube(D3DXVECTOR3 centerPos, float size);
	// ���μ�⣬����������������ǣ���Ĭ�Ͼ����� XOY ƽ��ƽ�У������Ǵ�ֱ��Y�������ϵ�����
	BOOL CheckRectangle(D3DXVECTOR3 centerPos, float size);
	// �������
	BOOL CheckSphere(D3DXVECTOR3 centerPos, float radius);

protected:
	D3DXPLANE m_planes[6];
};

