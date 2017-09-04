

#include "Effect.h"

Effect::Effect()
{
	pEffect = nullptr;
}

Effect::~Effect()
{
	if (pEffect!=nullptr)
	{
		pEffect->Release();
		pEffect = nullptr;
	}
}

HRESULT Effect::CreateFromFile(std::string filepath)
{
	if (pEffect != nullptr)
	{
		return S_FALSE;
	}

	Direct3D& d3d = Direct3D::GetInstance();

	HRESULT result=d3d.CreateEffectFromFile(*this,filepath);
	return result;
}

