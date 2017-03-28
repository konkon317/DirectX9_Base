#include "FbxMeshLoader.h"

#include "fbxUtil.h"

FbxMeshLoader::FbxMeshLoader()
{
	pVertexPoints_DX = NULL;
	
	vertexPointCount =0;

	polygonCount = 0;
	/*polygonVertexNum = 0;
	pIndexArray = NULL;*/
	pPolygonVertexCount = NULL;
	ppPolygonVertexIndex = NULL;

	pNormalCounts = NULL;


	ppNormalVector =NULL;

	loaded = false;
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
		for (int i = 0; i < polygonCount; i++)
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


	if(ppNormalVector!=NULL)
	{ 
		for (int i = 0; i < normalLayerCount; i++)
		{
			if (ppNormalVector[i] != NULL)
			{
				delete[] ppNormalVector[i];
				ppNormalVector[i] = NULL;
			}
		}
		delete[] ppNormalVector;
		ppNormalVector = NULL;
	}

	if (pNormalCounts != NULL)
	{
		delete[]pNormalCounts;
		pNormalCounts = NULL;
	}


	loaded = false;
}

void FbxMeshLoader::Load(FbxNode* pNode)
{

	if (loaded == false)
	{
		if (IsMesh(pNode))
		{
			//ノードからメッシュデータ取得
			FbxMesh *pMesh = pNode->GetMesh();

			if (pMesh != NULL)
			{
				loaded = true;

				std::cout << std::endl;
				std::cout << "mesh load "<<std::endl;

				

				//メッシュを構成するポリゴン数
				polygonCount = pMesh->GetPolygonCount();

				//メッシュの総頂点
				/*polygonVertexNum = pMesh->GetPolygonVertexCount();
				pIndexArray = pMesh->GetPolygonVertices();*/

				//ポリゴンを構成する頂点の数
				pPolygonVertexCount = new int[polygonCount];
				ppPolygonVertexIndex = new int*[polygonCount];

				for (int i = 0; i < polygonCount; i++)
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
				vertexPointCount = pMesh->GetControlPointsCount();

				FbxVector4* pControllPoints_FBX = pMesh->GetControlPoints();

				pVertexPoints_DX = new D3DXVECTOR4[vertexPointCount];

				//FbxVector4 から D3DXVECTOR4へ
				for (int i = 0; i < vertexPointCount; i++)
				{
					pVertexPoints_DX[i].x = (float)pControllPoints_FBX[i][0];
					pVertexPoints_DX[i].y = (float)pControllPoints_FBX[i][1];
					pVertexPoints_DX[i].z = (float)pControllPoints_FBX[i][2];
					pVertexPoints_DX[i].w = (float)pControllPoints_FBX[i][3];
				}			
				
				//------法線----

				//法線セットの数
				int normalLayerCount = pMesh->GetElementNormalCount();

				pNormalCounts = new int[normalLayerCount];
				ppNormalVector = new D3DXVECTOR4*[normalLayerCount];				

				for (int layer = 0; layer < normalLayerCount; layer++)
				{

					FbxGeometryElementNormal* pNormal= pMesh->GetElementNormal(layer);

					//マッピングモードの取得
					FbxGeometryElement::EMappingMode mappingMode = pNormal->GetMappingMode();
					//リファレンスモードの取得
					FbxGeometryElement::EReferenceMode referenceMode = pNormal->GetReferenceMode();

					//マッピングモードの判別
					switch (mappingMode)
					{
					case FbxGeometryElement::eByControlPoint:

						//リファレンスモードの判別
						switch(referenceMode)
						{ 
						case FbxGeometryElement::eDirect:					

							//eDirectの場合データは順番に格納されているためそのまま保持

							pNormalCounts[layer] = pNormal->GetDirectArray().GetCount();

							ppNormalVector[layer] = new D3DXVECTOR4[pNormalCounts[layer]];

							for (int i = 0; i < pNormalCounts[layer]; i++)
							{
								FbxVector4 vec = pNormal->GetDirectArray().GetAt(i);

								ppNormalVector[layer][i].x = static_cast<float>( vec[0]);
								ppNormalVector[layer][i].y = static_cast<float>(vec[1]);
								ppNormalVector[layer][i].z = static_cast<float>(vec[2]);
								ppNormalVector[layer][i].w = static_cast<float>(vec[3]);
							}

							break;

						case FbxGeometryElement::eIndexToDirect:
					
							break;
						}

						break;

					case FbxGeometryElement::eByPolygonVertex:
						//法線がポリゴンの各頂点ごとに設定されている場合						

						//リファレンスモードの判別
						switch (referenceMode)
						{
						case FbxGeometryElement::eDirect:
					
							//eDirectの場合データは順番に格納されているためそのまま保持				

							pNormalCounts[layer] = pNormal->GetDirectArray().GetCount();

							ppNormalVector[layer] = new D3DXVECTOR4[pNormalCounts[layer]];

							for (int i = 0; i < pNormalCounts[layer]; i++)
							{
								FbxVector4 vec = pNormal->GetDirectArray().GetAt(i);

								ppNormalVector[layer][i].x = static_cast<float>(vec[0]);
								ppNormalVector[layer][i].y = static_cast<float>(vec[1]);
								ppNormalVector[layer][i].z = static_cast<float>(vec[2]);
								ppNormalVector[layer][i].w = static_cast<float>(vec[3]);
							}
													
							break;

						case FbxGeometryElement::eIndexToDirect:
					
							break;
						}
						
						break;
					}
				}



				//読み込んだ情報を表示
				std::cout << "polygonCount : " << polygonCount << std::endl;
				std::cout << "polygonVertexList" << std::endl;
				
				for (int i = 0; i < polygonCount; i++)
				{
					std::cout << "PolygonVertexCount [ " << i << " ] : " << pPolygonVertexCount[i] <<" - ";
					std::cout << "vertexIndex ";
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