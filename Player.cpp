#include "Player.h"
#include "Field.h"


//const = 定義　　　static = 静的（複数newをされても同じ物を使う）
static const float VO = -10.0;


void Player::Image_Load()
{
}

//コンストラクター
//最初に一回だけ必ず呼ばれる
Player::Player() : GameObject()
{
	hImage = LoadGraph("絶望☆異世界転移/イラスト/仮プレイヤー.jpg");
	x = 200;
	y = 0;
	//velocity = 0
	y = 500;
	velocity = -10;
	Ground = false;

}

//デクストラクター
//最後に一回だけ必ず呼ばれる
Player::~Player()
{
}

//計算・処理するところ
void Player::Update()
{
	if (CheckHitKey(KEY_INPUT_D)) {
		x += 2;
	}

	if (CheckHitKey(KEY_INPUT_A)) {
		x -= 2;
	}
	
	if (onGround == true) {

		if (CheckHitKey(KEY_INPUT_SPACE)) {
			velocity = VO;
		}
	}

	y += velocity;
	Field* field = FindGameObject<Field>();
	int ret = field->HitCheck(x, y + 64);
	if (ret > 0) {
		y -= ret;
		velocity = 0;
		onGround = false;
	}
}

//表示するところ
void Player::Draw()
{
	DrawRectGraph(x, y, 0, 0, 64, 64, hImage, 1);

}
