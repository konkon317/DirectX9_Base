#include "Light.h"

Light::Light()
{
	
	ZeroMemory(&light, sizeof(D3DLIGHT9));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.r = 1.0f;
	light.Diffuse.g = 1.0f;
	light.Diffuse.b = 1.0f;
	light.Direction = D3DXVECTOR3(-0.5f, -1.0f, 0.5f);
	light.Range = 1000.0f;
}

void Light::Init(D3DXVECTOR4 dir,int num)
{
	direction = dir;

	lightNum = num;

	light.Direction= D3DXVECTOR3(dir.x,dir.y,dir.z);

	Direct3D& d3d = Direct3D::GetInstance();
	d3d.SetLight(0, light);
	d3d.LightEnable(0, TRUE);

}