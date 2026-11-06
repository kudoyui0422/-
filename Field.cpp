#include "Field.h"
#include"DxLib.h"
#pragma once

void Field::Image_Load() 
{

}
Field::Field()
{//画像サイズ510×510（1280*720=560,,,560/2=280)
	hImage = LoadGraph("イラスト/haikei.jpg");
	x = 280;
	y = 0;

	//FieldBGM読み込み
	FieldBGM = LoadSoundMem("Sound/T.mp3");

	
}

Field::~Field()
{
	// 音声データの解放
	DeleteSoundMem(FieldBGM);
}

void Field::Update()
{

}

void Field::Draw()
{
	DrawExtendGraph(x, y, x + 720, 720, hImage, TRUE);
}

int Field::HitCheck(int px, int py)
{
	return 0;
}