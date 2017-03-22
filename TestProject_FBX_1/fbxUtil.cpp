#include "fbxUtil.h"

void ProbeNode(FbxNode* pNode)
{
	//ノード探査関数
	if (pNode)
	{
		int childCount = pNode->GetChildCount();
		std::cout << pNode->GetName() << " : " << childCount << " children. ";

		if (IsMesh(pNode))
		{
			std::cout << " [ Mesh ] ";
		}
		
		std::cout << std::endl;

		if (IsMesh(pNode))
		{	
			FbxMesh *pMesh = pNode->GetMesh();

			if (pMesh != NULL)
			{
				std::cout << "mesh load" << std::endl;
			}
		}


		if (childCount > 0)
		{			
			std::cout <<">>>>  childNode" << std::endl;
		
			for (int i = 0; i < childCount; i++)
			{
				ProbeNode(pNode->GetChild(i));
			}
			std::cout << "<<<<  backParent" << std::endl;
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