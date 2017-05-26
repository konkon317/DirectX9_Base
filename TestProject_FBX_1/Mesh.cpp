#include "Mesh.h"


//--------------

//コンストラクタ
Mesh::Mesh()
{

}

//--------------

//デストラクタ
Mesh::~Mesh()
{

}

//--------------

//解放関数
void Mesh::Release()
{ 


}

void Mesh::TestFunc()
{
	TriangleList tl;
	TriangleList* p = &tl;
	tlList.push_back(tl);

	if (tlList.size() <= 0)
	{
		return;
	}


	//先ほど追加した要素を取得
	TL_List::iterator it = tlList.end();
	it--;
	// list.end()は最終要素ではなく終わりを示すだけのダミーなので
	//end()の一つ前が直前に追加した要素

	//itでやるよりポインターの方が若干やりやすいと感じるため アドレスを取得
	TriangleList* pTl = it->GetThisPointer();

	if (!pTl->TryAllocMem_Verticies(12))
	{
		tlList.erase(it);
		return;
	}

}