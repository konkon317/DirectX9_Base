#pragma once

#include <assert.h>

#include <windows.h>

#include <iostream>

#include <string>
#include <thread>
#include <mutex>



class SingletonFinalizer
{
public  :
	typedef void(*FinalizerFunc)();

	static void AddFinalizer(FinalizerFunc func);
	static void Finalize();

};

//Singletonクラスのテンプレート
//インスタンスが二つ以上作られないことを保証

template <class T>
class Singleton
{
public :
	
	//インスタンスの取得
	static T& GetInstance()
	{
		std::call_once(initFlag, Create);
		assert(pInstance);
		return *pInstance;
	}
	
protected:


	Singleton()
	{
	/*	char szBuff[256];
		wsprintf(szBuff, "SINGLETON template %s",typeid(*this).name());
		std::string st = szBuff;
		MessageBox(NULL, st.c_str(), TEXT("作成"), MB_OK);*/
	}

	virtual ~Singleton()
	{
		/*char szBuff[256];
		wsprintf(szBuff, "SINGLETON template %s", typeid(*this).name());
		std::string st = szBuff;
		MessageBox(NULL, st.c_str(), TEXT("開放"), MB_OK);*/
	}//デストラクタ

	//
	static bool IsInstatnceCreated(){ return (pInstance != nullptr); }

private:
	//インスタンスを指すポインタ
	static T* pInstance;

	static std::once_flag initFlag;

	//作成関数
	static  void Create()
	{
		pInstance = new T;
		SingletonFinalizer::AddFinalizer(&Singleton<T>::Releace);
	}

	//開放関数
	static void Releace()
	{
		delete pInstance;
		pInstance = nullptr;
	}

	//代入演算とコピーコンストラクタをprivateにして外部から実行できなくする
	void operator =(const Singleton& obj){}
	Singleton(const Singleton &obj){}

	
};

template <class T>std::once_flag Singleton<T>::initFlag;
template <class T>T* Singleton<T>::pInstance = nullptr;