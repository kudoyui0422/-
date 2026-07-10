#include "Player.h"
#include "Field.h"
#include "NPC.h"
#include <cmath> //距離演算に必要

//const = 定義　　　static = 静的（複数newをされても同じ物を使う）

static const int MOVE_SPEED = 2;

//会話ができる距離（ピクセル単位）
static const int TALK_RANGE = 100;


void Player::Image_Load()
{
}

//コンストラクター
//最初に一回だけ必ず呼ばれる
Player::Player()
{
	hImage = LoadGraph("イラスト/Player.png");

	x = 580;
	y = 0;
	//velocity = 0
	y = 500;  //初期位置
	velocity = -10;
	onGround = false;

	isTalking = false;
	oldKeyEnter = false;
}

//デクストラクター
//最後に一回だけ必ず呼ばれる
Player::~Player()
{
	//画像の開放
	DeleteGraph(hImage);
}



//計算・処理するところ
void Player::Update(NPC& npc)
{
	//現在のエンターキーの入力状態を取得
	bool currentKeyEnter = (CheckHitKey(KEY_INPUT_RETURN) == 1);

	//今押されたか、かつ会話中じゃない場合
	if (currentKeyEnter && !oldKeyEnter)
	{
		if (currentKeyEnter)
		{
			//会話中なら終了
			isTalking = false;
		}
		else {
			//ここにMobの距離判定を入れる
			float playerCenterX = x + 64.0f;
			float playerCenterY = y + 64.0f;
			float npcCenterX = npc.GetX() + 64.0f;
			float npcCenterY = npc.GetY() + 64.0f;
			//ここにMobの座標
			// 2点間の距離の2乗を計算（三平方の定理。高速化のためsqrtは省略）
			float dx = playerCenterX - npcCenterX;
			float dy = playerCenterY - npcCenterY;
			float distanceSq = (dx * dx) + (dy * dy);
		
			// 一定範囲内にNPCがいれば会話を開始
			if (distanceSq <= (TALK_RANGE * TALK_RANGE))
			{
				isTalking = true;
			}
		}
	}

	//次のフレームのために現在の入力状態を記憶しておく
	oldKeyEnter = currentKeyEnter;

	//会話中はここから下の移動処理をスキップ
	if (isTalking)
	{
		return;
	}

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

	//NPCとの当たり判定
	//AABB判定
	float pLeft = x + 40.0f;
	float pRight = x + 128.0f - 40.0f;
	float pTop = y + 112.0f;  // 上から112px下げて、足元16px分にする
	float pBottom = y + 128.0f; 

	float nLeft = npc.GetX() + 40.0f;
	float nRight = npc.GetX() + npc.GetWindth() - 40.0f;
	float nTop = npc.GetY() + npc.GetHeight() - 16.0f;
	float nBottom = npc.GetY() + npc.GetHeight();

	//4方向全て重なっている場合、衝突している
	if (pRight > nLeft && pLeft < nRight && pBottom > nTop && pTop < nBottom)
	{
		//どの方向から一番浅くめり込んでいるかを計算して、その方向に押し戻す
		float overlapLeft = pRight - nLeft;
		float overlapRight = nRight - pLeft;
		float overlapTop = pBottom - nTop;
		float overlapBottom = nBottom - pTop;

		//X軸とY軸のどっちのめり込みが小さいか比較
		float minOverlapX = (overlapLeft < overlapRight) ? overlapLeft : overlapRight;
		float minOverlapY = (overlapTop < overlapBottom) ? overlapTop : overlapBottom;


		if (minOverlapX < minOverlapY)
		{
			// X軸方向に押し戻す
			if (overlapLeft < overlapRight) {
				x -= overlapLeft; // 左側にぶつかったので左に押し戻す
			}
			else {
				x += overlapRight; // 右側にぶつかったので右に押し戻す
			}
		}
		else
		{

			// Y軸方向に押し戻す
			if (overlapTop < overlapBottom) {
				y -= overlapTop; // 上側にぶつかったので上に押し戻す
			}
			else {
				y += overlapBottom; // 下側にぶつかったので下に押し戻す
			}
		}
	}


	//フィールドの移動制限
	//左はし
	if (x < 280)
	{
		x = 280;
	}

	//右はし
	if (x > 872)
	{
		x = 872;
	}

	//上
	if (y < 0)
	{
		y = 0;
	}

	//下
	if (y > 592)
	{
		y = 592;
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
