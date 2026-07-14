#pragma once
#include"DxLib.h"


class Prologue
{
private:
	int sfxTruck; //トラックの音
	int prologueStep; //進行ド（0：文字表示、1：音再生中）
	bool oldKeyReturn; //wんたーきーの重複防止

public:
	Prologue(); //コンストラクター（音の読み込みや初期化）
	~Prologue(); //デストラクター（音の解放）

	//戻り値でメインループに進行情報を伝える
	//0:プロローグ継続、1：プロローグ終了（ゲーム本篇へ）
	int Update();
	void Draw();

};

