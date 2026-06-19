#include "Player.h"
#include "Field.h"
#include "DxLib.h"

//const = 定義　　　static = 静的（複数newをされても同じ物を使う）
static const float VO = -10.0;

//移動速度の定数
static const int MOVE_SPEED = 2;

void Player::Image_Load()
{
}

//コンストラクター
//最初に一回だけ必ず呼ばれる
Player::Player()
{
	hImage = LoadGraph("絶望/イラスト/仮プレイヤー.jpg");
	x = 200;
	//velocity = 0
	y = 500;  //初期位置


}

//デクストラクター
//最後に一回だけ必ず呼ばれる
Player::~Player()
{
	//画像の開放
	DeleteGraph(hImage); 
}

//計算・処理するところ
void Player::Update()
{
	//Dキーで右方向に移動
	if (CheckHitKey(KEY_INPUT_D)) {
		x += MOVE_SPEED;
	}

	//Aキーで左方向に移動
	if (CheckHitKey(KEY_INPUT_A)) {
		x -= MOVE_SPEED;
	}
	
	//Wキーで上方向に移動
	if (CheckHitKey(KEY_INPUT_W)) {
		y -= MOVE_SPEED;
	}

	//Sキーで下方向に移動
	if (CheckHitKey(KEY_INPUT_S)) {
		y += MOVE_SPEED;
	}


	
	
}

//表示するところ
void Player::Draw() const
{
	DrawRectGraph(x, y, 0, 0, 64, 64, hImage, 1);

}
