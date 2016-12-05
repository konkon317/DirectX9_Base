#pragma once

#include "Direct3D.h"
class Camera;

class Mesh
{
private:
	LPD3DXMESH pMesh;

public:
	Mesh();
	~Mesh();

	void DrawMatrice(D3DXMATRIXA16& mat_transform, D3DXMATRIXA16& mat_scale, D3DXMATRIXA16& mat_rotate);
	
	void Load(TCHAR* path);
};