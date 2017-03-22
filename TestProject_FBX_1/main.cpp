#include <iostream>
#include <fbxsdk.h>

#include "fbxUtil.h"

using namespace fbxsdk;

void WaitKey(char* message)
{
	std::cout << message << std::endl;
	std::cout << "Return キーを押してください" << std::endl;
	getchar();
}

int main(int argc, char* argv[])
{
	//------------------------------------------------------------------
	//FBX Managerの作成
	std::cout << "Create FbxManager ... ";
	FbxManager* pManager = FbxManager::Create();

	if (pManager == NULL)
	{
		std::cout << "Failed" << std::endl;

		WaitKey("プログラムの終了");
		return 0;
	}
	else
	{
		std::cout << "Succeded" << std::endl;
	}

	//------------------------------------------------------------------
	//FBX Iporterの作成
	std::cout << "Create FbxImporter ... ";
	FbxImporter* pImporter = FbxImporter::Create(pManager, "");
	
	if (pImporter == NULL)
	{
		std::cout << "Failed" << std::endl;
		pManager->Destroy();
		WaitKey("プログラムの終了");
	}
	else
	{
		std::cout << "Succeded" << std::endl;
	}
	
	//------------------------------------------------------------------
	//インポーターでファイルを開く
	const char* filePath = "FBX_FILES/cube.fbx";

	std::cout << "Open File [ " << filePath << " ] ... ";
	if (pImporter->Initialize(filePath) == false)
	{
		std::cout << "Failed " << std::endl;	
		pImporter->Destroy();
		pManager->Destroy();

		WaitKey("プログラムの終了");
		return 0;
	}
	else
	{
		std::cout << "Succeded " << std::endl;
	}

	//------------------------------------------------------------------
	//シーンの作成
	std::cout << "CreateScene ...";
	FbxScene* pScene = FbxScene::Create(pManager, "");

	if (pScene == NULL)
	{
		std::cout << "Failed " << std::endl;
		pImporter->Destroy();
		pManager->Destroy();
		
		WaitKey("プログラムの終了");
		return 0;
	}
	else
	{
		std::cout << "Succeded " << std::endl;
	}
	
	
	//------------------------------------------------------------------
	//インポーターからシーンを読み込む
	std::cout << "Import Scene ... " << std:: endl;

	if(!pImporter->Import(pScene))
	{
		std::cout << "Failed " << std::endl;
		pScene->Destroy();
		pImporter->Destroy();
		pManager->Destroy();

		WaitKey("プログラムの終了");
		return 0;
	}
	else
	{
		std::cout << "Succeded " << std::endl;
	}


	//シーンを読んだら必要ないのでインポーターは破棄する
	pImporter->Destroy();
	pImporter = NULL;
	
	//------------------------------------------------------------------
	//ノード探査
	FbxNode* pRootNode = pScene->GetRootNode();

	if (pRootNode)
	{		
		ProbeNode(pRootNode);		
	}

	pRootNode->Destroy();

	//------------------------------------------------------------------
	std::cout << "終了処理に入ります" <<std::endl;

	
	pScene->Destroy();
	pManager->Destroy();

	WaitKey("プログラムの終了");
	return 1;
		

}