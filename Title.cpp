#include "Title.h"
#include "Screen.h" //画面サイズ（Screen::WIDTHなどをつかうため）
#include "DxLib.h"

Title::Title()
{
	//画像読み込み
	hTitleBack = LoadGraph("イラスト/タイトル画像.jpg");
	hButtonStart = LoadGraph("イラスト/はじめるボタン.png");
	hButtonEnd = LoadGraph("イラスト/おわるボタン.png");

	//ボタンの配置座標
	startX = 680;
	startY = 250;

	endX = 680;
	endY = startY + buttonHeight - 30; //30は間の広さ

	oldMouseClick = false;
}

Title::~Title()
{
	//画像の解放漏れを防ぐ（デストラクター）
	DeleteGraph(hTitleBack);
	DeleteGraph(hButtonStart);
	DeleteGraph(hButtonEnd);

}

int Title::Update()
{
	int mouseX, mouseY;
	GetMousePoint(&mouseX, &mouseY);
	bool currentMouseClick = ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0);
	 
	//マウスがボタンの上にあるかを判定
	bool isHoverStart = (mouseX >= startX && mouseX <= startX + buttonWidth && mouseY >= startY && mouseY <= startY + buttonHeight);
	bool isHoverEnd = (mouseX >= endX && mouseX <= endX + buttonWidth && mouseY >= endY && mouseY <= endY + buttonHeight);

	int result = 0; //0:変化なし

	//クリックされた瞬間の判定
	if (currentMouseClick && !oldMouseClick)
	{
		if (isHoverStart) result = 1; //1:ゲーム開始
		if (isHoverEnd) result = 2; //2:ゲーム終了
	}

	oldMouseClick = currentMouseClick;
	return result;	
}

void Title::Draw()
{
	//背景の描画
	DrawExtendGraph(0, 0, Screen::WIDTH, Screen::HEIGHT, hTitleBack, FALSE);

	//マウスの位置を取得してホバー演出
	int mouseX, mouseY;
	GetMousePoint(&mouseX, &mouseY);
	bool isHoverStart = (mouseX >= startX && mouseX <= startX + buttonWidth && mouseY >= startY && mouseY <= startY + buttonHeight);
	bool isHoverEnd = (mouseX >= endX && mouseX <= endX + buttonWidth && mouseY >= endY && mouseY <= endY + buttonHeight);

	//はじめるボタン描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, isHoverStart ? 255 : 200);
	DrawGraph(startX, startY, hButtonStart, TRUE);

	//おわるボタン描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, isHoverEnd ? 255 : 200);
	DrawGraph(endX, endY, hButtonEnd, TRUE);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//通常の描画モードに戻す

}
