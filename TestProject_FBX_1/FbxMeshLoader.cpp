#include "FbxMeshLoader.h"

#include "fbxUtil.h"

FbxMeshLoader::FbxMeshLoader()
{
	pVertexPoints_DX = NULL;
	
	controlNum=0;

	polygonNum = 0;
	polygonVertexNum = 0;
	pIndexArray = NULL;
	pPolygonVertexCount = NULL;
	ppPolygonVertexIndex = NULL;
}

FbxMeshLoader::~FbxMeshLoader()
{
	Release();
}

void FbxMeshLoader::Release()
{
	if (pVertexPoints_DX != NULL)
	{
		delete[] pVertexPoints_DX;
		pVertexPoints_DX = NULL;
	}

	if (ppPolygonVertexIndex != NULL)
	{
		for (int i = 0; i < polygonNum; i++)
		{
			if (ppPolygonVertexIndex[i] != NULL)
			{
				delete[] ppPolygonVertexIndex[i];
			}
		}

		delete[] ppPolygonVertexIndex;
		ppPolygonVertexIndex = NULL;
	}

	if (pPolygonVertexCount != NULL)
	{
		delete[] pPolygonVertexCount;
		pPolygonVertexCount = NULL;;
	}
}

void FbxMeshLoader::Load(FbxNode* pNode)
{

	if (Loaded == false)
	{
		if (IsMesh(pNode))
		{
			//ノードからメッシュデータ取得
			FbxMesh *pMesh = pNode->GetMesh();

			if (pMesh != NULL)
			{
				Loaded = true;

				std::cout << " mesh load "<<std::endl;

				//メッシュを構成するポリゴン数
				polygonNum = pMesh->GetPolygonCount();

				//メッシュの総頂点
				polygonVertexNum = pMesh->GetPolygonVertexCount();
				pIndexArray = pMesh->GetPolygonVertices();

				//ポリゴンを構成する頂点の数
				pPolygonVertexCount = new int[polygonNum];
				ppPolygonVertexIndex = new int*[polygonNum];

				for (int i = 0; i < polygonNum; i++)
				{
					pPolygonVertexCount[i] = 0;
					ppPolygonVertexIndex[i] = NULL;

					//ポリゴン[i]を構成する頂点数
					pPolygonVertexCount[i] = pMesh->GetPolygonSize(i);

					//ポリゴン[i]を構成する頂点のインデックスを格納する
					ppPolygonVertexIndex[i] = new int[pPolygonVertexCount[i]];

					for (int j = 0; j < pPolygonVertexCount[i]; j++)
					{
						ppPolygonVertexIndex[i][j] = pMesh->GetPolygonVertex(i, j);
					}
				}
				

				//頂点座標の数
				controlNum = pMesh->GetControlPointsCount();

				FbxVector4* pControllPoints_FBX = pMesh->GetControlPoints();

				pVertexPoints_DX = new D3DXVECTOR4[controlNum];

				//FbxVector4 から D3DXVECTOR4へ
				for (int i = 0; i < controlNum; i++)
				{
					pVertexPoints_DX[i].x = pControllPoints_FBX[i][0];
					pVertexPoints_DX[i].y = pControllPoints_FBX[i][1];
					pVertexPoints_DX[i].z = pControllPoints_FBX[i][2];
					pVertexPoints_DX[i].w = pControllPoints_FBX[i][3];
				}
				

				//読み込んだ情報を表示
				std::cout << "polygonNum : " << polygonNum << std::endl;
				std::cout << "polygonVertexList" << std::endl;
				
				for (int i = 0; i < polygonNum; i++)
				{
					std::cout << "PolygonVertexCount [ " << i << " ] : " << pPolygonVertexCount[i] << std::endl;
					std::cout << "vertexIndex";
					for (int j = 0; j < pPolygonVertexCount[i]; j++)
					{
						std::cout << ppPolygonVertexIndex[i][j]<<",";
					}
					std::cout << std::endl;
				}

				pMesh->Destroy();
			}
		}
	}
}