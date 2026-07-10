#pragma once
#include "DxLib.h"
#include "Player.h"

class Field;

class NPC
{
	void Image_Load();

public:
	NPC(); //コンストラクター（初期化）
	~NPC(); //デクストラクター（解放）
	void Update(); //計算・更新処理
	void Draw(); //描画処理


	//main等からFieldのアドレスを受け取る
	void SetField(Field* f) { field = f; }
	int GetX() const { return x; }
	int GetY() const { return y; }
	int GetWindth() const { return 128; } //NPCの横幅
	int GetHeight() const { return 128; } //NPCの立幅


private:
	int playerGraph;  //画像の識別番号
	int hImage;
	float x, y;
	float velocity;
	bool onGround;
	Field* field; //前方宣言をしたことで、エラーがなくなる

};

