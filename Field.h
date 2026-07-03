#pragma once
#include "Field.h"
#include "DxLib.h"

class Field
{

	void Image_Load();
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

