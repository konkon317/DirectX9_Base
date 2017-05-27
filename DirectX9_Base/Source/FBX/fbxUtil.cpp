#include "fbxUtil.h"
#include "fbxMeshLoader.h"
#include "../Model3D/Model.h"
#include <iomanip>


void FbxUtil::ProbeNode(FbxNode* pNode,Model* pModel,int depth)
{
	//ノード探査関数
	if (pNode)
	{
		int childCount = pNode->GetChildCount();
	

		//for (int i = 0; i < depth; i++)
		//{
		//	std::cout << " " ;
		//}
		//
		//std::cout << "|- ("<<std::setw(4)<<depth<<") " ;		

		//std::cout << pNode->GetName() << " : " << childCount << " children. ";

		//if (IsMesh(pNode))
		//{
		//	std::cout << " [ Mesh ] ";
		//}			

		if (IsMesh(pNode))
		{	
			FbxMeshLoader fbxMeshLoader;
			fbxMeshLoader.Load(pNode);

			if (pModel != nullptr)
			{
				//モデルにメッシュの情報を追加する
				pModel->AddMeshFromFbxMeshLoader(&fbxMeshLoader);
			}
		}

		//std::cout << std::endl;

		if (childCount > 0)
		{			
			//std::cout <<">>>>  In childNode" << std::endl;
		
			for (int i = 0; i < childCount; i++)
			{
				ProbeNode(pNode->GetChild(i),pModel, depth + 1);
			}
			//std::cout << "<<<<  back ParentNode" << std::endl;
		}
	}
}



bool FbxUtil::IsMesh(FbxNode* pNode)
{
	//ノードがメッシュに繋がっているか

	//ノードには複数のアトリビュートが繋がっているため、その個数分チェックする

	if (pNode)
	{
		int attrCount = pNode->GetNodeAttributeCount();

		for (int i = 0; i < attrCount; i++)
		{

			FbxNodeAttribute::EType attrType = pNode->GetNodeAttributeByIndex(i)->GetAttributeType();

			if (attrType == FbxNodeAttribute::EType::eMesh)
			{
				return true;
			}			
		}
	}	

	return false;
}



bool FbxUtil::ReadModelFromFbx(Model& model, const char* filePath)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//------------------------------------------------------------------
	//FBX Managerの作成
	
	//std::cout << "Create FbxManager ... ";
	FbxManager* pManager = FbxManager::Create();

	if (pManager == nullptr)
	{
		//std::cout << "Failed" << std::endl;

		//WaitKey("プログラムの終了");
		return false;
	}
	else
	{
		//std::cout << "Succeded" << std::endl;
	}

	//------------------------------------------------------------------
	//FBX Iporterの作成

	//std::cout << "Create FbxImporter ... ";
	FbxImporter* pImporter = FbxImporter::Create(pManager, "");


	if (pImporter == nullptr)

	{
		//std::cout << "Failed" << std::endl;
		pManager->Destroy();
		//WaitKey("プログラムの終了");
		return false;
	}
	else
	{
		//std::cout << "Succeded" << std::endl;
	}

	//------------------------------------------------------------------
	//インポーターでファイルを開く
	

	//std::cout << "Open File [ " << filePath << " ] ... ";

	if (pImporter->Initialize(filePath) == false)
	{
		//std::cout << "Failed " << std::endl;
		pImporter->Destroy();
		pManager->Destroy();

		//WaitKey("プログラムの終了");
		return false;
	}
	else
	{
		//std::cout << "Succeded " << std::endl;
	}

	//------------------------------------------------------------------
	//シーンの作成
	
	//std::cout << "CreateScene ...";
	
	FbxScene* pScene = FbxScene::Create(pManager, "");

	if (pScene == nullptr)
	{
		//std::cout << "Failed " << std::endl;
		
		pImporter->Destroy();
		pManager->Destroy();

		//WaitKey("プログラムの終了");
		return 0;
	}
	else
	{
		//std::cout << "Succeded " << std::endl;
	}


	//------------------------------------------------------------------
	//インポーターからシーンを読み込む
	
	//std::cout << "Import Scene ... " << std::endl;

	if (!pImporter->Import(pScene))
	{
		//std::cout << "Failed " << std::endl;
	
		pScene->Destroy();
		pImporter->Destroy();
		pManager->Destroy();

		//WaitKey("プログラムの終了");
		
		return false;
	}
	else
	{
		//std::cout << "Succeded " << std::endl;
	}


	//シーンを読んだら必要ないのでインポーターは破棄する
	pImporter->Destroy();
	pImporter = nullptr;

	//------------------------------------------------------------------
	//ノード探査
	FbxNode* pRootNode = pScene->GetRootNode();

	if (pRootNode)
	{
		//モデルを探査して見つけたメッシュやマテリアルをモデルに追加する
		ProbeNode(pRootNode, &model);
	}

	pRootNode->Destroy();

	//------------------------------------------------------------------
	
	//std::cout << "終了処理に入ります" << std::endl;


	pScene->Destroy();
	pManager->Destroy();

	return true;
}