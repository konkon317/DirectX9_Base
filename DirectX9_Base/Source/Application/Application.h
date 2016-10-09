#pragma once

#include <Windows.h>
#include "../Template/Singleton.h"


class Application :public Singleton<Application>
{
	friend class Singleton<Application>;
public :

	

protected:


private:
	Application();
	~Application(); //デストラクタのオーバーロード


};
