#pragma once
#include <string>
#include "DxLib.h"
#include "Player.h"

class Field;

class NPC
{
	void Image_Load();



private:
	int playerGraph;  //画像の識別番号
	int hImage;
	float x, y;
	float velocity;
	bool onGround;
	Field* field; //前方宣言をしたことで、エラーがなくなる

	//メッセージのページ番号を管理する変数
	int messageIndex;

public:
	NPC(); //コンストラクター（初期化）
	~NPC(); //デクストラクター（解放）
	void Update(); //計算・更新処理
	void Draw(); //描画処理

	//messageIndexに応じたセリフを返すように関数化
	std::string GetMessage();

	//メッセージを次のページに進める関数
	void AdvanceMessage() { messageIndex++; }

	//会話が終わった時や、最初から話すときのためにリセット
	void ResetMessade() { messageIndex = 0; }


	//main等からFieldのアドレスを受け取る
	void SetField(Field* f) { field = f; }
	int GetX() const { return x; }
	int GetY() const { return y; }
	int GetWindth() const { return 128; } //NPCの横幅
	int GetHeight() const { return 128; } //NPCの立幅

};

