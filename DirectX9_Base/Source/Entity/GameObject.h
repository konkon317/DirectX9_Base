#pragma once 


class GameObject
{
public :
	virtual ~GameObject(){};

	virtual void Update(){};
	virtual void Draw(){};
};