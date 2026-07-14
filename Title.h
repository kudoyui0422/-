#pragma once
#include"DxLib.h"

class Title
{
private:
	int hTitleBack; //背景画像
	int hButtonStart; //はじめるボタン
	int hButtonEnd; //おわるボタン

	int buttonWidth = 430; //ボタンの横幅
	int buttonHeight = 200; //ボタンの縦幅

	//ボタンたちの配置座標
	int startX, startY;
	int endX, endY;

	bool oldMouseClick; //前フレームのクリック状態


public:
	Title(); //コンストラクター（画像の読み込みや座標計算）
	~Title(); //デストラクター（画像の解放）

	//戻り値でmウィンループに「シーンの切替」と「終了」を伝える
	// 0:タイトル継続、1：ゲーム開始、　2：ゲーム終了
	int Update();
	void Draw();

};

