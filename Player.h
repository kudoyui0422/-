#pragma once

//DXライブラリの関数（LoadGraphなどを使うため）
#include "DxLib.h"

//前方宣言
class Field;
class NPC;

class Player 
{
	// 画像を読み込むための関数を「宣言」する
	void Image_Load();

private:
	int playerGraph;  //画像の識別番号
	int hImage;
	int hImageBack; //後ろ向き画像
	int hImageLeft; //左向き画像
	int hImageRight; //右向き画像
	int direction; //プレイヤーの向き(0:前、1：後ろ、2：左、3；右)

	float x, y;
	float velocity;
	bool onGround;
	Field* field; //前方宣言をしたことで、エラーがなくなる

	bool isTalking; //現在会話中かどうか
	bool oldKeyEnter; //前のフレームでエンターキーが押されていたか
	bool GetTalking() const { return isTalking; }

public:
	Player();  //コンストラクター（初期化）
	~Player();  //デクストラクター（解放）
	void ForceStartTalk();
	void Update(NPC& npc);
	void Draw();  //描画処理

	//main等からFieldのアドレスを受け取る
	void SetField(Field* f) { field = f; }
	int GetX() const { return x; }
	int GetY() const { return y; }

	//mainから会話状態をチェック出来るようにpublicに配置
	bool GetIsTalking() const { return isTalking; }

	void SetOldKeyEnter(bool flag) { oldKeyEnter = flag; }

};

