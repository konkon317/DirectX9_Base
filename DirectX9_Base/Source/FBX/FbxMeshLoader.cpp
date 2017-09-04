#include "FbxMeshLoader.h"

#include "fbxUtil.h"
#include <string>


#include <stack>
#include <sstream>
#include <iostream>


using namespace std;



UvSet::UvSet()
{
	pUvBuffer = nullptr;
}


UvSet::~UvSet()
{
	if (pUvBuffer != nullptr)
	{
		delete[] pUvBuffer;
		pUvBuffer = nullptr;
	}
}



FbxMeshLoader::FbxMeshLoader()
{
	pControlPoints_DX = nullptr;
	
    controlPointCount =0;

	polygonCount = 0;
	/*polygonVertexNum = 0;
	pIndexArray = NULL;*/
	pPolygonVertexCount = nullptr;
	ppPolygonVertexIndex = nullptr;

	pNormalCounts = nullptr;


	ppNormalVector = nullptr;


	uvSetCount = 0;

	indexCount = 0;
	pVertexPoints_DX = nullptr;
	
	pUvSetArray = nullptr;

	loaded = false;
}

FbxMeshLoader::~FbxMeshLoader()
{
	Release();
}

void FbxMeshLoader::Release()
{
	if (pControlPoints_DX != nullptr)
	{
		delete[] pControlPoints_DX;
		pControlPoints_DX = nullptr;
	}


	//ポリゴンに使ったメモリの解放
	if (ppPolygonVertexIndex != nullptr)
	{
		for (int i = 0; i < polygonCount; i++)
		{
			if (ppPolygonVertexIndex[i] != nullptr)
			{
				delete[] ppPolygonVertexIndex[i];
			}
		}

		delete[] ppPolygonVertexIndex;
		ppPolygonVertexIndex = nullptr;
	}

	if (pPolygonVertexCount != nullptr)
	{
		delete[] pPolygonVertexCount;
		pPolygonVertexCount = nullptr;
	}

	if (pVertexPoints_DX != nullptr)
	{
		delete[] pVertexPoints_DX;
		pVertexPoints_DX = nullptr;
		indexCount = 0;

	}



	//法線に使ったメモリの解放
	if(ppNormalVector!= nullptr)
	{ 
		for (int i = 0; i < normalLayerCount; i++)
		{
			if (ppNormalVector[i] != nullptr)
			{
				delete[] ppNormalVector[i];
				ppNormalVector[i] = nullptr;
			}
		}
		delete[] ppNormalVector;
		ppNormalVector = nullptr;
	}

	if (pNormalCounts != nullptr)
	{
		delete[]pNormalCounts;
		pNormalCounts = nullptr;
	}

	//頂点色に使ったメモリの解放
	if (ppVertexColor != nullptr)
	{
		for (int i = 0; i <  VertexColorSetMax; i++)
		{
			if (ppVertexColor[i] != nullptr)
			{
				delete[] ppVertexColor[i];
				ppVertexColor[i] = nullptr;
			}
		}
		delete[]ppVertexColor;
		ppVertexColor = nullptr;
	}

	if (pColorCount_ByVertexColorSet != nullptr)
	{
		delete[]pColorCount_ByVertexColorSet;
		pColorCount_ByVertexColorSet = nullptr;
	}

	//UVセットの解放
	if (pUvSetArray != nullptr)
	{
		delete[] pUvSetArray;
		pUvSetArray = nullptr;
	}

	loaded = false;
}


//ロード
void FbxMeshLoader::Load(FbxNode* pNode,string filepath)
{

	if (loaded == false)
	{
		if (FbxUtil::IsMesh(pNode))
		{
			//ノードからメッシュデータ取得
			FbxMesh *pMesh = pNode->GetMesh();

			if (pMesh != nullptr)
			{
				loaded = true;

				/*std::cout << std::endl;
				std::cout << "mesh load "<<std::endl;	*/
								
				//頂点座標
				LoadVertexPosition(pMesh);

				//頂点色の取得
				LoadVertexColor(pMesh);

				//法線の取得
				LoadNormal(pMesh);

				//UVの取得
				LoadVertexUV_Buffer(pMesh);
				
				//UVSEtとマテリアルの関連付け
				Asociate_UVSetAndMaterial(pNode,filepath);		

				////読み込んだ情報を表示
				//std::cout << "polygonCount : " << polygonCount << std::endl;
				//std::cout << "polygonVertexList" << std::endl;
				//
				//for (int i = 0; i < polygonCount; i++)
				//{
				//	std::cout << "PolygonVertexCount [ " << i << " ] : " << pPolygonVertexCount[i] <<" - ";
				//	std::cout << "vertexIndex ";
				//	for (int j = 0; j < pPolygonVertexCount[i]; j++)
				//	{
				//		std::cout << ppPolygonVertexIndex[i][j]<<",";
				//	}
				//	std::cout << std::endl;
				//}

				pMesh->Destroy();			
			}			
		}		
	}	
}


void FbxMeshLoader::LoadVertexPosition(FbxMesh* pMesh)
{
	//メッシュを構成するポリゴン数
	polygonCount = pMesh->GetPolygonCount();

	//メッシュの総頂点		

	//ポリゴンを構成する頂点の数
	pPolygonVertexCount = new int[polygonCount];
	ppPolygonVertexIndex = new int*[polygonCount];

	bool flag = false;
	for (int i = 0; i < polygonCount; i++)
	{
		pPolygonVertexCount[i] = 0;
		ppPolygonVertexIndex[i] = nullptr;

		//ポリゴン[i]を構成する頂点数
		pPolygonVertexCount[i] = pMesh->GetPolygonSize(i);

		//ポリゴン[i]を構成する頂点のインデックスを格納する
		ppPolygonVertexIndex[i] = new int[pPolygonVertexCount[i]];

		for (int j = 0; j < pPolygonVertexCount[i]; j++)
		{
		
			ppPolygonVertexIndex[i][j] = pMesh->GetPolygonVertex(i, j);
		}

		//四角形以上のポリゴンがあった
		if (pPolygonVertexCount[i] > 3)
		{
			flag = true;
		}
	}

	//四角形以上のポリゴンが1つ以上あった
	//if (flag)
	//{
	//	std::cout << "メッシュロード中　四角形以上のポリゴンが含まれていた" << std::endl;
	//	WaitKey("多角形ポリゴンが含まれていました");
	//}


	//頂点座標の数
	controlPointCount = pMesh->GetControlPointsCount();

	FbxVector4* pControllPoints_FBX = pMesh->GetControlPoints();

	pControlPoints_DX = new D3DXVECTOR4[controlPointCount];

	//FbxVector4 から D3DXVECTOR4へ
	for (int i = 0; i < controlPointCount; i++)
	{
		pControlPoints_DX[i].x = static_cast<float>(pControllPoints_FBX[i][0]);
		pControlPoints_DX[i].y = static_cast<float>(pControllPoints_FBX[i][1]);
		pControlPoints_DX[i].z = static_cast<float>(pControllPoints_FBX[i][2]);
		pControlPoints_DX[i].w = static_cast<float>(pControllPoints_FBX[i][3]);
	}


	indexCount = pMesh->GetPolygonVertexCount();
	
	pVertexPoints_DX = new D3DXVECTOR4[indexCount];

	
	int i = 0;

	for (int polygon = 0; polygon < polygonCount; polygon++)
	{
		
		for (int j = 0; j < pPolygonVertexCount[polygon]; j++)
		{

			int index = ppPolygonVertexIndex[polygon][j];//コントロール点の番号


			pVertexPoints_DX[i] = pControlPoints_DX[index];
			i++;
		}
	}

	{
		int breakPoint=0;
	}

}

//UV座標取得関数
void FbxMeshLoader::LoadVertexUV_Buffer(FbxMesh* pMesh)
{
	//http://shikemokuthinking.blogspot.jp/

	//UVSetの数を取得
	
	uvSetCount = pMesh->GetElementUVCount();
	pUvSetArray = new UvSet[uvSetCount];

	for (int i = 0; i<uvSetCount; i++)
	{
		//Uvバッファを取得
		FbxGeometryElementUV*UV = pMesh->GetElementUV(i);


		//マッピングモードの取得
		FbxGeometryElement::EMappingMode mapping = UV->GetMappingMode();

		//リファレンスモードの取得
		FbxGeometryElement::EReferenceMode reference = UV->GetReferenceMode();


		//UV数の取得
		
		FbxLayerElementArrayTemplate<int>* pUvIndex = &UV->GetIndexArray();
		int uvIndexCount = pUvIndex->GetCount();

		pUvSetArray[i].uvSetName = UV->GetName();

		pUvSetArray[i].pUvBuffer = new Point2DF[uvIndexCount];
	
		//int index = 0;
		//for (int p = 0; p < this->polygonCount; p++)
		//{
		//	for (int pv = 0; pv < pPolygonVertexCount[p]; pv++)
		//	{	
		//		bool unmapped = false;
		//		FbxVector2 v;
		//		pMesh->GetPolygonVertexUV(p,pv,UV->GetName(),v,unmapped);

		//		if (!unmapped)
		//		{
		//			Point2DF temp;				

		//			temp.x = static_cast<float>(v[0]);
		//			temp.y = static_cast<float>(1.0f - v[1]);//ブレンダーで作ったファイルは上下逆

		//			pUvSetArray[i].pUvBuffer[index] = temp;
		//		}

		//		index++;
		//	}

		//}
	
		switch (mapping)
		{
			case fbxsdk::FbxLayerElement::eByControlPoint:
				{
					int BreakPoint = 0;
				}

				break;
			case fbxsdk::FbxLayerElement::eByPolygonVertex:

				switch (reference)
				{
					case FbxGeometryElement::eDirect:
						{
							int BreakPoint = 0;
						}
						break;

					case FbxGeometryElement::eIndexToDirect:
						{
							for (int j = 0; j < uvIndexCount; j++)
							{
								Point2DF temp;

								int index = pUvIndex->GetAt(j);
								FbxVector2 v = UV->GetDirectArray().GetAt(index);

								temp.x =static_cast<float>( v[0] );
								temp.y =static_cast<float>( 1.0f- v[1]);//ブレンダーで作ったファイルは上下逆

								pUvSetArray[i].pUvBuffer[j] = temp;
							}

							//UvSet名を取得
							pUvSetArray[i].uvSetName = UV->GetName();
							pUvSetArray[i].uvCount = uvIndexCount;
							{
								int BreakPoint = 0;
							}

						}
				
						break;
				}

				break;

			case fbxsdk::FbxLayerElement::eByPolygon:
				{
					int BreakPoint = 0;
				}
				break;
			case fbxsdk::FbxLayerElement::eByEdge:

				{
					int BreakPoint = 0;
				}
				break;

			case fbxsdk::FbxLayerElement::eAllSame:

				{
					int BreakPoint = 0;
				}
				break;

			default:
				{
					int BreakPoint = 0;
				}
				break;
		}

		UV->Destroy();
	}
}


//法線取得関数
void FbxMeshLoader::LoadNormal(FbxMesh* pMesh)
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

//UVとマテリアルのテクスチャを関連付ける関数
void  FbxMeshLoader::Asociate_UVSetAndMaterial(FbxNode* pNode,std::string filepath)
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
						std::string relPath = pTexture->GetRelativeFileName();

						//UVSet名を取得
						std::string UVSetName = pTexture->UVSet.Get().Buffer();

						for (int k=0;k<uvSetCount;k++)
						{
							if (pUvSetArray[k].uvSetName == UVSetName)
							{
							

								pUvSetArray[k].texture = FbxUtil::MakeTexturePath(filepath, relPath);
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
						std::string relPath = pTexture->GetRelativeFileName();
						
						//--- UVSet名を取得 ---//
						std::string UVSetName = pTexture->UVSet.Get().Buffer();

						//--- UVSet名を比較し対応しているテクスチャなら保持 ---//
						for (int k = 0; k<uvSetCount; k++)
						{
							if (pUvSetArray[k].uvSetName == UVSetName)
							{
								/*stack<string> filepathStack;

								{
									std::istringstream stream(filepath);
									string s;

									while (std::getline(stream, s, '/'))
									{
										filepathStack.push(s);
									}
								}

								stack<string>relPathStack;
								{
									std::istringstream stream(tes);
									string s;

									while (std::getline(stream, s, ''))
									{
										relPathStack.push(s);
									}
								}*/

								pUvSetArray[k].texture = FbxUtil::MakeTexturePath(filepath,relPath);
							}
						}
					}
				}
			}
		}
		pMaterial->Destroy();
	}
}


//頂点色取得関数
void FbxMeshLoader::LoadVertexColor(FbxMesh* pMesh)
{
	//頂点カラーセット数を取得
	VertexColorSetMax = pMesh->GetElementVertexColorCount();

	ppVertexColor = new ColorRGBA*[VertexColorSetMax];
	pColorCount_ByVertexColorSet = new int[VertexColorSetMax];

	for (int i = 0; i < VertexColorSetMax; i++)
	{
		ppVertexColor[i] = nullptr;
		pColorCount_ByVertexColorSet[i] = 0;
	}

	//カラーセットの数だけ繰り返す
	for (int i = 0; i <VertexColorSetMax; i++)
	{
		//頂点カラーセットを取得
		FbxGeometryElementVertexColor * pColor = pMesh->GetElementVertexColor(i);

		FbxGeometryElement::EMappingMode mappingMode = pColor->GetMappingMode();
		FbxGeometryElement::EReferenceMode referenceMode = pColor->GetReferenceMode();

		switch (mappingMode)
		{

			case FbxGeometryElement::eByControlPoint:
				{
					int BreakPoint = 0;
				}
				break;

			case FbxGeometryElement::eByPolygon:
				{
					int BreakPoint = 0;
				}
				break;

			case FbxGeometryElement::eByPolygonVertex:

				switch (referenceMode)
				{

					case fbxsdk::FbxLayerElement::eIndexToDirect:
						{
							//今回はまず mapping =eByPolygonVertex reference=eIndexToCirect
							//の組み合わせから書く 
							//実験用のファイルがその保存形式なので

							FbxLayerElementArrayTemplate<int>* pIndex = &pColor->GetIndexArray();


							pColorCount_ByVertexColorSet[i] = pIndex->GetCount();


							ppVertexColor[i] = new ColorRGBA[pColorCount_ByVertexColorSet[i]];

							//頂点の数だけ頂点カラーを取得
							for (int j = 0; j < pColorCount_ByVertexColorSet[i]; j++)
							{

								int index = pIndex->GetAt(j);
								FbxColor color = pColor->GetDirectArray().GetAt(index);

								ppVertexColor[i][j].r = static_cast<float>(color[0]);
								ppVertexColor[i][j].g = static_cast<float>(color[1]);
								ppVertexColor[i][j].b = static_cast<float>(color[2]);
								ppVertexColor[i][j].a = static_cast<float>(color[3]);
							}
							{
								int BreakPoint = 0;
							}

						}
						break;

					case fbxsdk::FbxLayerElement::eDirect:
						{
							int BreakPoint = 0;
						}
						break;

					case fbxsdk::FbxLayerElement::eIndex:
						{
							int BreakPoint = 0;
						}
						break;

					default:
						{
							int BreakPoint = 0;
						}
						break;
				}

				break;

			default:

				{
					int BreakPoint = 0;
				}

				break;
		}
	}
	

}


