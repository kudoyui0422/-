#pragma once
#include "Field.h"


class Field
{
public:
	Field();
	~Field();
	void Update();
	void Draw();

	int HitCheck(int px, int py);

private:
	int hImage;
	float x, y;

};

