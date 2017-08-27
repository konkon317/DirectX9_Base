#include "meshX.h"


MeshX::MeshX()
{
	pMesh = nullptr;
	
}

MeshX::~MeshX()
{
	for (unsigned int i = 0; i < numMaterials; i++)
	{
		if (ppTextures[i])
		{
			ppTextures[i]->Release();
		}
	}

	delete[] ppTextures;
	delete[] pMaterials;
	
	pMesh->Release();
}

void MeshX::DrawMatrice( D3DXMATRIXA16& mat_transform, D3DXMATRIXA16& mat_scale, D3DXMATRIXA16& mat_rotate,Effect* pEffect )
{
	D3DXMATRIXA16 matWorld;

	//座標　拡大　回転　を保持する行列
	D3DXMATRIXA16 mat_temp;

	//単位行列に初期化
	D3DXMatrixIdentity(&matWorld);
	
	D3DXMatrixIdentity(&mat_temp);

	////モデルの配置
	//static float testx = 0.0f;
	////testx += 0.01f;
	
	////モデルの拡大率
	//static float testScaleY = 0;
	//testScaleY += 0.005f;
	

	D3DXMatrixMultiply(&mat_temp, &mat_scale, &mat_rotate);   //一時行列に拡大率 回転を合成したものを代入
	D3DXMatrixMultiply(&matWorld, &mat_temp, &mat_transform); //一時行列と座標を合成


	
	Direct3D::GetInstance().DrawMesh(*this,  matWorld,pEffect);

}



void MeshX::Load(TCHAR* path)
{
	Direct3D::GetInstance().LoadMesh(*this, path);

}