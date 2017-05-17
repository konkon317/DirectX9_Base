#include "fbxUtil.h"
#include "fbxMeshLoader.h"
#include <iomanip>

void ProbeNode(FbxNode* pNode,int depth)
{
	//ノード探査関数
	if (pNode)
	{
		int childCount = pNode->GetChildCount();
		for (int i = 0; i < depth; i++)
		{
			std::cout << " " ;
		}
		
		std::cout << "|- ("<<std::setw(4)<<depth<<") " ;		

		std::cout << pNode->GetName() << " : " << childCount << " children. ";

		if (IsMesh(pNode))
		{
			std::cout << " [ Mesh ] ";
		}			

		if (IsMesh(pNode))
		{	
			FbxMeshLoader fbxMeshLoader;
			fbxMeshLoader.Load(pNode);
		}

		std::cout << std::endl;

		if (childCount > 0)
		{			
			//std::cout <<">>>>  In childNode" << std::endl;
		
			for (int i = 0; i < childCount; i++)
			{
				ProbeNode(pNode->GetChild(i), depth + 1);
			}
			//std::cout << "<<<<  back ParentNode" << std::endl;
		}
	}
}



bool IsMesh(FbxNode* pNode)
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