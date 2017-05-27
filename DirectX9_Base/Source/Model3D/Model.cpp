#include "Model.h"



//--------------

//コンストラクタ
Model::Model()
{

}

//--------------

//デストラクタ
Model::~Model()
{

}

//--------------

//解放関数
void Model::Release()
{


}


//--------------

bool Model::AddMeshFromFbxMeshLoader(FbxMeshLoader* pMeshLoader)
{
	
	Mesh mesh;
	meshList.push_back(mesh);

	if (meshList.size() <= 0)
	{
		return false;
	}

	//先ほど追加した要素を取得
	Mesh_List::iterator it = meshList.end();
	it--;
	// list.end()は最終要素ではなく終わりを示すだけのダミーなので
	//end()の一つ前が直前に追加した要素

	//itでやるよりポインターの方が若干やりやすいと感じるため アドレスを取得
	Mesh* pMesh = it->GetThisPointer();

	if (pMesh->Load(pMeshLoader))
	{
		meshList.erase(it);
		return false;
	}		
	

	return true;
}


//--------------

//デバッグ用の関数

void Model::Debug_TestShow()
{
	int bp = 0;
	int a = meshList.size();
	bp = 0;

	for (Mesh_List::iterator it = meshList.begin(); it != meshList.end(); it++)
	{
		it->Debug_TestShow();
	}
};