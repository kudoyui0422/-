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

	x = 580;
	y = 150;

	//最初は0ページから
	messageIndex = 0;

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

//ページ番号に応じたセルフを返す
std::string NPC::GetMessage()
{
	if (messageIndex == 0)
	{
		return"「　ようこそ異世界へ！　\n　ここは絶望に満ちた世界だよ。」";
	}
	else if (messageIndex == 1)
	{
		return"「元の世界に帰りたい？ぼくが手伝ってあげる。」";
	}
	else if (messageIndex == 2)
	{
		return"「どうやって帰るの？。」"; //主人公の問い返し
	}
	else if (messageIndex == 3)
	{
		return"「今から簡単な質問をするから　\n　君はそれにこたえるだけでいいよ。」"; //選択して前の問い
	}
	return "";
}
