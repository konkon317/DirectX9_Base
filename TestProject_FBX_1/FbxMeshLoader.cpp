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
					pVertexPoints_DX[i].x = static_cast<float>(pControllPoints_FBX[i][0]);
					pVertexPoints_DX[i].y = static_cast<float>(pControllPoints_FBX[i][1]);
					pVertexPoints_DX[i].z = static_cast<float>(pControllPoints_FBX[i][2]);
					pVertexPoints_DX[i].w = static_cast<float>(pControllPoints_FBX[i][3]);
				}			
			



				//法線の取得
				GetNormal(pMesh);

				//UVの取得
				GetVertexUV_Buffer(pMesh);

				
				//UVSEtとマテリアルの関連付け
				Asociate_UVSetAndMaterial(pNode);
				



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






void FbxMeshLoader::GetVertexUV_Buffer(FbxMesh* pMesh)
{
	//http://shikemokuthinking.blogspot.jp/

	//UVSetの数を取得
	int UvLayerCount = pMesh->GetElementUVCount();
	
	UvSet uvSet;
	for (int i = 0; i<UvLayerCount; i++)
	{
		//Uvバッファを取得
		FbxGeometryElementUV*UV = pMesh->GetElementUV(i);


		//マッピングモードの取得
		FbxGeometryElement::EMappingMode mapping = UV->GetMappingMode();

		//リファレンスモードの取得
		FbxGeometryElement::EReferenceMode reference = UV->GetReferenceMode();


		//UV数の取得
		int uvCount = UV->GetDirectArray().GetCount();

		switch (mapping)
		{
		
		case fbxsdk::FbxLayerElement::eByControlPoint:

			break;
		case fbxsdk::FbxLayerElement::eByPolygonVertex:
		{
			switch (reference)
			{
			case FbxGeometryElement::eDirect:
				break;
			case FbxGeometryElement::eIndexToDirect:
			{
				FbxLayerElementArrayTemplate<int>* pUvIndex = &UV->GetIndexArray();
				int uvIndexCount = pUvIndex->GetCount();

				Point2DF temp;
				for (int i = 0; i < uvIndexCount; i++)
				{
					temp.x = (float)UV->GetDirectArray().GetAt(pUvIndex->GetAt(i))[0];
					temp.y = 1.0f - (float)UV->GetDirectArray().GetAt(pUvIndex->GetAt(i))[1];//ブレンダーで作ったファイルは上下逆

					uvSet.uvBuffer.push_back(temp);
				}

				//UvSet名を取得
				uvSet.uvSetName = UV->GetName();

				list_uvSet.push_back(uvSet);
			}
			break;

			}

		}
			break;
		
		case fbxsdk::FbxLayerElement::eByPolygon:
			break;
		case fbxsdk::FbxLayerElement::eByEdge:
			break;
		case fbxsdk::FbxLayerElement::eAllSame:
			break;
		default:
			break;
		}
		UV->Destroy();

	}
}

void FbxMeshLoader::GetNormal(FbxMesh* pMesh)
{
	//------法線----

	//法線セットの数
	normalLayerCount = pMesh->GetElementNormalCount();

	pNormalCounts = new int[normalLayerCount];
	ppNormalVector = new D3DXVECTOR4*[normalLayerCount];

	for (int layer = 0; layer < normalLayerCount; layer++)
	{

		FbxGeometryElementNormal* pNormal = pMesh->GetElementNormal(layer);

		//マッピングモードの取得
		FbxGeometryElement::EMappingMode mappingMode = pNormal->GetMappingMode();
		//リファレンスモードの取得
		FbxGeometryElement::EReferenceMode referenceMode = pNormal->GetReferenceMode();

		//マッピングモードの判別
		switch (mappingMode)
		{
		case FbxGeometryElement::eByControlPoint:

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
		pNormal->Destroy();
	}
}

void  FbxMeshLoader::Asociate_UVSetAndMaterial(FbxNode* pNode)
{
	//注意作りかけ　2017_4_18

	//UVSetとマテリアルの関連付け
	//uvセットのテクスチャ名とマテリアルのテクスチャ名で関連付け

	//マテリアルの数を取得
	int materialCount = pNode->GetMaterialCount();

	for (int i = 0; i < materialCount; i++)
	{
		//マテリアルの取得
		FbxSurfaceMaterial * pMaterial = pNode->GetMaterial(i);

		//マテリアルからDeffuseを取得
		FbxProperty prop = pMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);


		//プロパティにアタッチされているテクスチャの種別を判断
		int LayerdTextureCount = prop.GetSrcObjectCount<FbxLayeredTexture>();

		//FbxLayerdTexture :複数のテクスチャをブレンド使用されている場合に有効になる
		//FbxFileTexture :　単一のテクスチャとして使用されていると有効になる
		//FbxProceduralTexture : 不明今回は考慮しない

		if (LayerdTextureCount>0)
		{
			for (int j = 0; j < LayerdTextureCount; j++)
			{
				//テクスチャを取得
				FbxLayeredTexture *pLayerdTexture = prop.GetSrcObject<FbxLayeredTexture>(j);

				//レイヤー数を取得
				int textureCount = pLayerdTexture->GetSrcObjectCount<FbxFileTexture>();

				for (int k = 0; k < textureCount; k++)
				{
					FbxFileTexture* pTexture = prop.GetSrcObject<FbxFileTexture>(k);


					if (pTexture)
					{
						//テクスチャ名を取得
						std::string textureName = pTexture->GetRelativeFileName();

						//UVSet名を取得
						std::string UVSetName = pTexture->UVSet.Get().Buffer();

						for (UVSetList::iterator it = list_uvSet.begin(); it != list_uvSet.end(); it++)
						{
							if ((*it).uvSetName == UVSetName)
							{
								(*it).texture = textureName;
							}
						}
					}
				}
			}
		}
		else
		{
			//--- テクスチャ数を取得 ---//
			int fileTextureCount = prop.GetSrcObjectCount<FbxFileTexture>();

			if (0 < fileTextureCount)
			{
				//--- テクスチャの数だけ繰り返す ---//
				for (int j = 0; fileTextureCount > j; j++)
				{
					//--- テクスチャを取得 ---//
					FbxFileTexture* pTexture = prop.GetSrcObject<FbxFileTexture>(j);
					if (pTexture)
					{
						//--- テクスチャ名を取得 ---//
						//std::string textureName = texture->GetName();
						std::string textureName = pTexture->GetRelativeFileName();

						//--- UVSet名を取得 ---//
						std::string UVSetName = pTexture->UVSet.Get().Buffer();

						//--- UVSet名を比較し対応しているテクスチャなら保持 ---//
						for (UVSetList::iterator it = list_uvSet.begin(); it != list_uvSet.end(); it++)
						{
							if ((*it).uvSetName == UVSetName)
							{
								(*it).texture = textureName;
							}
						}
					}
				}
			}
		}
		pMaterial->Destroy();
	}
}


