// 絶望☆異世界転移.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//
#include<DxLib.h>
#include <iostream>
#include "Screen.h"
#include"Player.h"
#include"Field.h"
#include"NPC.h"
#include"Title.h"
#include"Prologue.h"

enum GameState
{
    STATE_TITLE,
    STATE_PROLOGUE,
    STATE_GAME
};

int main()
{
    ChangeWindowMode(Screen::WINDOW_MODE);
    SetGraphMode(Screen::WIDTH, Screen::HEIGHT, 32);
    SetOutApplicationLogValidFlag(FALSE);
    SetMainWindowText(Screen::WINDOW_NAME);

    if (Screen::WINDOW_MODE == TRUE)
    {
        SetWindowSizeExtendRate(Screen::WINDOW_EXTEND);
    }

    if (DxLib_Init() == -1) return -1;
    SetDrawScreen(DX_SCREEN_BACK);
    SetMouseDispFlag(TRUE); // 最初はマウスを表示

    // 各クラスのインスタンスを作る
    Title title;
    Prologue prologue; // 💡 プロローグのインスタンスを作成！
    Player player;
    Field field;
    NPC npc;

    player.SetField(&field);
    npc.SetField(&field);

    int hNPCFace = LoadGraph("イラスト/NPCアイコン.png");

    GameState state = STATE_TITLE;

    while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
    {
        ClearDrawScreen();

       
        // ① タイトル画面の処理
       
        if (state == STATE_TITLE)
        {
            int titleResult = title.Update();
            title.Draw();

            if (titleResult == 1)
            {
                state = STATE_PROLOGUE; // プロローグへ進む
                SetMouseDispFlag(FALSE); // マウスを隠す
            }
            else if (titleResult == 2)
            {
                break; // ゲーム終了
            }
        }
   
        // ② プロローグ画面の処理（Prologueクラスに丸投げ！）
  
        else if (state == STATE_PROLOGUE)
        {
            int prologueResult = prologue.Update(); // 計算を丸投げ
            prologue.Draw();                       // 描画を丸投げ

            if (prologueResult == 1)
            {
                state = STATE_GAME; // 本編へ進む
                player.ForceStartTalk(); // 主人公の独り言を強制開始
            }
        }  
        
        // ③ ゲーム本編の処理
     
        else if (state == STATE_GAME)
        {
            player.Update(npc);
            field.Draw();

            if (player.GetY() < npc.GetY()) { player.Draw(); npc.Draw(); }
            else { npc.Draw(); player.Draw(); }

            if (player.GetIsTalking())
            {
                SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
                DrawBox(280, 580, 1000, 720, GetColor(0, 0, 0), TRUE);
                SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

                DrawBox(280, 580, 1000, 720, GetColor(255, 255, 255), FALSE);
                DrawExtendGraph(290, 590, 290 + 120, 590 + 120, hNPCFace, TRUE);

                std::string text = "「ここはどこだ？！」";
                float dx = (player.GetX() + 64) - (npc.GetX() + 64);
                float dy = (player.GetY() + 120) - (npc.GetY() + 120);
                if ((dx * dx + dy * dy) <= (130.0f * 130.0f))
                {
                    text = npc.GetMessage();
                }

                SetFontSize(24);
                DrawString(430, 600, text.c_str(), GetColor(255, 255, 255));
                SetFontSize(16);
            }
        }

        ScreenFlip();
    }

    DeleteGraph(hNPCFace);
    DxLib_End();
    return 0;
}
