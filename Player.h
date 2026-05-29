#pragma once

//DXライブラリの関数（LoadGraphなどを使うため）
#include "DxLib.h"


class Player 
{

	// 画像を読み込むための関数を「宣言」する
	void Image_Load();
public:
	Player();  //コンストラクター（初期化）
	~Player();  //デクストラクター（解放）
	void Image_Load();  //画像の読み込み処理
	void Update();  //計算・更新処理
	void Draw();  //描画処理

private:
	int playerGraph;  //画像の識別番号
	int hImage;
	float x, y;
	float velocity;
	bool onGround;
	Field* field;

	// 他のクラスからプレイヤーの座標を取得するための関数（必要に応じて使用）
	int GetX() const { return x; }
	int GetY() const { return y; }
};

