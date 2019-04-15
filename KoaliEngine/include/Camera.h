//======================================================================
//	Desc: 摄像机类		
//=======================================================================

#pragma once
#include"D3dUtility.h"

class Camera
{
public:
	enum CameraType { LANDOBJECT, AIRCRAFT };

	Camera();
	Camera(CameraType cameraType);
	Camera(D3DXVECTOR3 pos);
	~Camera();

	void strafe(float units); // left/right
	void fly(float units);    // up/down
	void walk(float units);   // forward/backward

	void pitch(float angle); // rotate on right vector
	void yaw(float angle);   // rotate on up vector
	void roll(float angle);  // rotate on look vector

	void getViewMatrix(D3DXMATRIX* V);
	void setCameraType(CameraType cameraType);
	void getCameraPosition(D3DXVECTOR3* pos);
	void setCameraPosition(D3DXVECTOR3* pos);

	float GetPosX();
	float GetPosY();
	float GetPosZ();

	void getRight(D3DXVECTOR3* right);
	void getUp(D3DXVECTOR3* up);
	void getLook(D3DXVECTOR3* look);

	void SetTargetPosition(D3DXVECTOR3 *pLookat = NULL);  //设置摄像机的目标观察位置向量

	void ThirdPersonRotationX(float angle);					//世界坐标的X向量
	void ThirdPersonRotationY(float angle);					//世界坐标的Y向量

	void set3DCamera(IDirect3DDevice9 *device);				//设置3D摄像机	

protected:
	CameraType  _cameraType;
	D3DXVECTOR3 _right;
	D3DXVECTOR3 _up;
	D3DXVECTOR3 _look;
	D3DXVECTOR3 _cameraPosition;
	D3DXVECTOR3 _targetPosition;
};

