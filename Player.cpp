#include "Player.h"
#include "Field.h"


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
	hImage = LoadGraph("イラスト/Player.png");
	
	x = 200;
	y = 0;
	//velocity = 0
	y = 500;  //初期位置
	velocity = -10;
	onGround = false;
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

	//地面・壁などの当たり判定
	
	if (field != nullptr) {
		//見下ろし型だから足元の座標（x、y＋128)障害物判定を行う
		//field->HitCheckの戻り値に合わせて、めり込みを押し戻す処理を入れる
		int ret = field->HitCheck(x, y + 128);
		if (ret > 0) {
			y -= ret;
	    }
	
	}
}

//表示するところ
void Player::Draw()
{
	DrawRectGraph(x, y, 0, 0, 128, 128, hImage, 1);

}
