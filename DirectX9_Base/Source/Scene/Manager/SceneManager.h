#pragma once 

#include <typeinfo>

#include <map>
using namespace std;


//シーン　基底クラス
class Scene
{

public :
	Scene(){};
	virtual ~Scene(){};

	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Init() = 0;

};

class TestScene :public Scene
{
	
public :
	TestScene(){};
	~TestScene(){};

	virtual void Update(){};
	virtual void Draw(){};
	virtual void Init(){};
};

//シーンクリエイター
class SceneCreater
{

private:
	
	int targetScene_TypeHash;		

	//明示的にコンストラクタを呼べなくする
	
	SceneCreater(SceneCreater&){};
	SceneCreater& operator=(SceneCreater&){};

protected:
	bool doneHashSet;

	SceneCreater();

	void SetTargetSceneHash(int hash);
public :		
	
	int GetTargetSceneTypeHash(){ return targetScene_TypeHash; };

	virtual ~SceneCreater(){};

	virtual Scene* CreateScene() = 0;

};

class TestSceneCreater : public SceneCreater
{

private:
	static TestSceneCreater instance;

//protected:
	TestSceneCreater();

public :
	~TestSceneCreater();
	Scene* CreateScene();

};

class SceneManager
{
private:
	static map <int, SceneCreater*>* pSceneCreaterMap;

public :
	static void RegisterSceneCreater(SceneCreater* pSc);

	static void Finalize();


private:
	Scene* pCurrentScene;
	Scene* pNextScene;

	int currentSceneHash;
	int nextSceneHash;

public:
	SceneManager();
	void Update();

	void Draw();

	void ChangeScene(int nextHash);

};

