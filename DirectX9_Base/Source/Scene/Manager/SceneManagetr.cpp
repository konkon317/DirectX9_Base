#include "SceneManager.h"

TestSceneCreater TestSceneCreater::instance;

map <int, SceneCreater*>*  SceneManager::pSceneCreaterMap=nullptr;

Scene::Scene()
{
	
}

SceneCreater::SceneCreater()
{
	doneHashSet = false;
	targetScene_TypeHash = 0;
	
}

void SceneCreater::SetTargetSceneHash(int hash)
{ 
	if (doneHashSet == false)
	{
		targetScene_TypeHash = hash;
		doneHashSet = true;
	}
}


TestSceneCreater::TestSceneCreater()
{
	SetTargetSceneHash(typeid(TestScene).hash_code());

	if (doneHashSet == true)
	{
		SceneManager::RegisterSceneCreater(this);
	}
}

TestSceneCreater::~TestSceneCreater()
{

}

Scene* TestSceneCreater::CreateScene()
{
	Scene* pScene = new TestScene;
	return pScene; 
}

void SceneManager::RegisterSceneCreater(SceneCreater* pSc)
{
	int hash = pSc->GetTargetSceneTypeHash();

	if (pSceneCreaterMap == nullptr)
	{
		pSceneCreaterMap = new map <int, SceneCreater*>();
	}
	
	if (pSceneCreaterMap != nullptr)
	{
		if (pSceneCreaterMap->count(hash) == 0)
		{
			pSceneCreaterMap->insert(map<int, SceneCreater*>::value_type(hash,pSc));
		}
	}
}


void SceneManager::Finalize()
{
	

	if (pSceneCreaterMap != nullptr)
	{
		pSceneCreaterMap->clear();

		delete pSceneCreaterMap;
	}
}

SceneManager::SceneManager()
{
	pCurrentScene=nullptr;
	pNextScene=nullptr;

	currentSceneHash = 0;
	nextSceneHash = 0;

}

SceneManager::~SceneManager()
{
	if (pCurrentScene != nullptr ||pNextScene!=nullptr)
	{
		if (pNextScene != nullptr)
		{
			if (pCurrentScene != pNextScene)
			{
				delete pNextScene;
			}
			pNextScene = nullptr;
		}

		if (pCurrentScene != nullptr)
		{
			delete pCurrentScene;
			pCurrentScene = nullptr;
		}

	}
}

void SceneManager::Update()
{
	if (pSceneCreaterMap != nullptr)
	{
		if (nextSceneHash != currentSceneHash)
		{
			if (pNextScene == nullptr)
			{
				if (pSceneCreaterMap->count(nextSceneHash) > 0)
				{
					SceneCreater* pSC = (*pSceneCreaterMap)[nextSceneHash];
					if (pSC != nullptr)
					{
						pNextScene = pSC->CreateScene();
					}
				}
			}
		}

		if (pNextScene != nullptr && (pNextScene!=pCurrentScene))
		{
			if (pCurrentScene != nullptr)
			{
				delete pCurrentScene;
			}

			pCurrentScene = pNextScene;
			pNextScene = nullptr;

			currentSceneHash = nextSceneHash;
		}
	}

	if (pCurrentScene != nullptr)
	{
		pCurrentScene->Update();		
	}
}

void SceneManager::Draw()
{
	if (pCurrentScene != nullptr)
	{		
		pCurrentScene->Draw();
	}
}

void SceneManager::ChangeScene(int nextHash)
{
	if (pSceneCreaterMap != nullptr)
	{
		if (currentSceneHash != nextHash)
		{
			if (pSceneCreaterMap->count(nextHash)>0)
			{
				nextSceneHash = nextHash;
			}
		}
	}
}
	

