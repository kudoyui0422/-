#include "NPC.h"
#include "Player.h"
#include "Field.h"
#include "DxLib.h"

void NPC::Image_Load()
{

}
//コンストラクター
//必ず一回最初に呼ばれる

NPC::NPC()
{
	hImage = LoadGraph("イラスト/NPC.png");

	x = 350;
	y = 150;

}

//デストラクター
NPC::~NPC()
{
	//画像のメモリ解放漏れを防ぐ
	DeleteGraph(hImage);
}


void NPC::Update()
{
}

//表示するところ
void NPC::Draw()
{
	DrawRectGraph((int)x, (int)y, 0, 0, 128, 128, hImage, 1);
}