#include  "MeshXWithHeight.h"

#include "tchar.h"

IDirect3DVertexDeclaration9* MeshXWithHeight::pVertexDelc = nullptr;

D3DVERTEXELEMENT9 delc[] =
{
	{ 0,0,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_POSITION,0 },
	{ 0,12,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_NORMAL,0 },
	{ 0,24,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TANGENT,0 },
	{ 0,36,D3DDECLTYPE_FLOAT2,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,0 },
	D3DDECL_END()
};

void MeshXWithHeight::CreateDelc()
{

	if (pVertexDelc == nullptr)
	{
	


		Direct3D::GetInstance().CreateVertexDecle(delc, &pVertexDelc);
	}
}

MeshXWithHeight::MeshXWithHeight()
{
	ppTextureFileNames = nullptr;
	CreateDelc();
}

MeshXWithHeight::~MeshXWithHeight()
{
	if (ppNormalTextures)
	{
		for (int i = 0; i < numMaterials; i++)
		{
			if (ppNormalTextures[i])
			{
				ppNormalTextures[i]->Release();
			}
		}

		delete[] ppNormalTextures;
	}
}

void MeshXWithHeight::Load(TCHAR* path)
{
	MeshX::Load(path);
	if (pMesh)
	{
		if (numMaterials > 0 && ppTextureFileNames != nullptr)
		{
			ppNormalTextures = new LPDIRECT3DTEXTURE9[numMaterials];
			for (int i = 0; i < numMaterials; i++)
			{
				ppNormalTextures[i] = nullptr;
			}
			for (int i = 0; i < numMaterials; i++)
			{
				if (ppTextureFileNames[i] != nullptr)
				{
					CHAR dir[_MAX_DIR];
					_splitpath_s(path, NULL, 0, dir, _MAX_DIR, NULL, 0, NULL, 0);

					CHAR texturefile[1024];
					ZeroMemory(texturefile, sizeof(texturefile));

					CHAR FileName[128];
					ZeroMemory(FileName, sizeof(FileName));
					CHAR Extension[16];
					ZeroMemory(Extension, sizeof(Extension));

					int nameLen = lstrlen(ppTextureFileNames[i]);

					CHAR dot = _T('.');
					bool f = false;

					CHAR c[2];
					ZeroMemory(c, sizeof(c));

					for (int j = 0; j < nameLen; j++)
					{

						c[0] = ppTextureFileNames[i][j];

						if (f == false)
						{
							if (c[0] == dot)
							{
								f = true;
								lstrcat(Extension, c);
							}
							else
							{
								lstrcat(FileName, c);
							}
						}
						else
						{
							lstrcat(Extension, c);
						}
					}

					lstrcat(texturefile, dir);
					lstrcat(texturefile, FileName);
					lstrcat(texturefile, _T("Height"));
					lstrcat(texturefile, Extension);
					Direct3D::GetInstance().LoadNormalTextures(ppNormalTextures[i], texturefile);
				}

			}
		}


		LPD3DXMESH pMeshTmp;
		
		Direct3D::GetInstance().CloneMesh(pMesh, pMeshTmp, delc);

		D3DXComputeNormals(pMeshTmp, NULL);
		D3DXComputeTangent(pMeshTmp, 0, 0, 0, TRUE, NULL);//接ベクトルの計算

		pMesh->Release();
		pMesh = pMeshTmp;
	}
}