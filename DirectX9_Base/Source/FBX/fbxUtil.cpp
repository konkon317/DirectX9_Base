#include "fbxUtil.h"
#include "fbxMeshLoader.h"
#include "../Model3D/Model.h"
#include <iomanip>

#include <stack>
#include <queue>
#include <vector>

#include <sstream>

using namespace std;




void FbxUtil::ProbeNode(FbxNode* pNode,Model* pModel,string filepath,int depth)
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
			fbxMeshLoader.Load(pNode,filepath);

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
				ProbeNode(pNode->GetChild(i),pModel, filepath,depth + 1);
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
		ProbeNode(pRootNode, &model,filePath);
	}

	pRootNode->Destroy();

	//------------------------------------------------------------------
	
	//std::cout << "終了処理に入ります" << std::endl;


	pScene->Destroy();
	pManager->Destroy();

	return true;
}

string FbxUtil::MakeTexturePath(string fbxPath, string relpath)
{
	stack<string> filepathStack;//fbxファイルへのパスをスタックに格納したもの

	{
		//stirng からstreamを作る
		std::istringstream stream(fbxPath);
		string s;

		//区切り文字を改行の代わりにスラッシュ
		//スラッシュが出てくるまでを1ブロックとして
		//sに読み込み、ｓをスタックへ投入
		while (std::getline(stream, s, '/'))
		{
			filepathStack.push(s);
		}
	}

	//fbxファイルからのテクスチャへの相対パス
	//をキューに格納したもの
	queue<string>relPathQueue;

	{
		std::istringstream stream(relpath);
		string s;
		//上のスタックと同じ要領
		while (std::getline(stream, s, '\\'))
			//\\で\一文字を表す
			//fbxsdkから取得したファイルパスの区切りが'\'だから
			// '/'の代わりにこちらを使う
		{
			relPathQueue.push(s);
		}
	}

	// .fbx　ファイル名部分を取り除く
	//　フォルダの情報のみが残る
	filepathStack.pop();

	//filepathStackが一回でも空になったらtrueにする
	bool reachEmpty = (filepathStack.empty()) ? true : false;

	//ファイルパスの作成
	while (!relPathQueue.empty())//相対パスの待ち行列が空になるまで続ける
	{
		//待ち行列の先頭を取得
		//キューからの削除はされない
		string s = relPathQueue.front();

		//フォルダ階層を一つ戻る場合
		if (s == "..")
		{

			if (reachEmpty)
			{
				//fbxPathの情報が全てなくなっている場合は
				// ..　を付け加える
				filepathStack.push("..");
			}
			else
			{
				//fbxPathの情報がなくなっていない場合はひとつ消す
				filepathStack.pop();

				if (filepathStack.empty())
				{
					//空になっていないか調べる
					reachEmpty = true;
				}
			}
		}
		else
		{
			//フォルダ名ファイル名が出た場合は追加			
			filepathStack.push(s);
		}

		//相対パスのキューの先頭を削除
		relPathQueue.pop();
	}

	//スタックの上から取り出すと
	//ファイルパスが逆転した状態に成っていしまうため
	//一回ベクトルへ移す

	vector<string> v;

	while (!filepathStack.empty())
	{
		string s2 = filepathStack.top();
		filepathStack.pop();
		v.push_back(s2);
	}
	//この時点でvの中に逆転したファイルパスが入っている

	//ベクトルの中を逆順に見ていくのでendからスタート
	vector<string>::iterator  it = v.end();

	//完成のファイルパスを入れる変数
	string finalPath = "";

	//逆順でfinalPathの中に格納していく

	bool first = true;
	do
	{
		it--;//先にイテレータをマイナス　

			 //begin()のデータで作業を行った直後にループを終わりたいため

			 //end()のデータはダミーデータなので
			 //最初の一回目はここで有効なデータに合わせる

			 //最初の一回目だけ区切りの/の追加をしない
		if (first)
		{
			first = false;
		}
		else
		{
			finalPath += "/";
		}

		//ファイルパスにフォルダ情報を追加
		finalPath += *it;

	} while (it != v.begin());
	//先頭データまでファイルパスに追加したらループ終了

	return finalPath;
}