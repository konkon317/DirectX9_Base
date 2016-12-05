#pragma once

#include "direct3D.h"

enum CAMERA_MODE
{
	LOOK_AT_REL,//relative
	LOOK_AT_ABS,//absolute

};

class Camera
{
private:

	CAMERA_MODE cameraMode;

	D3DXVECTOR3 eyePoint;
	D3DXVECTOR3 relLookAtPoint;
	D3DXVECTOR3 absLookAtPoint;
	D3DXVECTOR3 lookAtPoint;
	D3DXVECTOR3 upVec;

	void UpdateLookAtPoint();

public:
	Camera();
	~Camera();

	D3DXVECTOR3* GetEyePoint_p(){ return &eyePoint; }
	D3DXVECTOR3* GetLookAtPoint_p(){ return &lookAtPoint; }
	D3DXVECTOR3* GetRelLookAtPoint_p(){ return &relLookAtPoint; }
	D3DXVECTOR3* GetAbsLookAtPoint_p(){ return &absLookAtPoint; }

	D3DXVECTOR3* GetUpVector_p(){ return &upVec; }

	void SetCameraMode(CAMERA_MODE cmode);

	void Move(float x, float y);
	void Rotate(float radYaw, float radPitch);

	void SetEyePoint(float x, float y, float z);
	void SetUpVector(float x, float y, float z);

	void SetAbsLookAtPoint(float x, float y, float z);
	void SetRelLookAtPoint(float x, float y, float z);

	void SetViewMatrix();

};