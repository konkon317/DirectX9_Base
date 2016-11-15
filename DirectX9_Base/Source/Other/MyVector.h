#pragma once

class Vector2
{
	int x;
	int y;

public:

	Vector2()
	{
		x = y = 0;
	}

	void Set(int x, int y)
	{
		this->x = x;
		this->y = y;
	}

	int X()const{ return x; }
	int Y()const{ return y; }


	//以下演算子のオーバーロード
	const Vector2 operator +(const Vector2& other)const
	{
		Vector2 temp;
		temp.Set(x + other.X(), y + other.Y());
		return temp;
	}

	const Vector2 operator -(const Vector2& other)const
	{
		Vector2 temp;
		temp.Set(x - other.X(), y - other.Y());
		return temp;
	}

	const Vector2& operator += (const Vector2& other)
	{
		x += other.X();
		y += other.Y();

		return *(this);
	}

	const Vector2& operator -= (const Vector2& other)
	{
		x -= other.X();
		y -= other.Y();

		return *(this);
	}
};
