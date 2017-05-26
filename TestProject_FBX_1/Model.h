#pragma once

#include "Global.h"
#include "Mesh.h"

#include "FbxMeshLoader.h"

typedef list<Mesh> Mesh_List ;

class Model
{
public :
	//コンストラクタ
	Model();

	//デストラクタ
	~Model();

private:
	//メンバ変数 private
	Mesh_List meshList;


public :
	//メンバ変数 public 





private:
	//メンバ関数 private



public :
	//メンバ関数 public 

	void Release();
	bool AddMeshFromFbxMeshLoader(FbxMeshLoader* pMeshLoader);

	void TestShow()
	{
		for(Mesh_List::iterator it =meshList.begin();it!=meshList.end();it++)
		{ 
			it->TestShow();
		}
	}

};