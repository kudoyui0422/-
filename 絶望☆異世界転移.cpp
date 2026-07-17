// 絶望☆異世界転移.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//
#include<string> //std::stringを使うのに必要
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
    Prologue prologue; // プロローグのインスタンスを作成
    Player player;
    Field field;
    NPC npc;

    player.SetField(&field);
    npc.SetField(&field);

    int hPlayerFace = LoadGraph("イラスト/Playerアイコン.jpg");
    int hNPCFace = LoadGraph("イラスト/NPCアイコン.png");

    // プロローグ終了後のウェイト用タイマー（60フレーム ＝ 約1秒）
    int prologueWaitTimer = 0;

    bool isFirstPrologueEnd = false;
    bool hasTalkedFirst = false; //独り言が終わったか

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
               

                player.SetOldKeyEnter(true); //プロローグが終わった後のエンター押しっぱを相殺

                // ここではまだ会話を開始せず、タイマーをリセットする
                prologueWaitTimer = 0;
                isFirstPrologueEnd = true;
            }
        }  
        
        // ③ ゲーム本編の処理
     
        else if (state == STATE_GAME)
        {
            // プロローグ直後かつ、まだ会話が始まっていない間はタイマーを毎フレーム進める
            if (isFirstPrologueEnd && !player.GetIsTalking())
            {
                prologueWaitTimer++;

                // 120フレーム（約2秒）経ったら会話を強制開始
                if (prologueWaitTimer >= 120)
                {
                    player.ForceStartTalk();
                    player.SetOldKeyEnter(true); // エンターキーの押しっぱなし相殺
                }
            }

            
            player.Update(npc);
            field.Draw();

            // 描画順の制御（Y座標で前後関係を決める）
            if (player.GetY() < npc.GetY()) { player.Draw(); npc.Draw(); }
            else { npc.Draw(); player.Draw(); }

            // 会話中の場合の処理
            if (player.GetIsTalking())
            {
                // ① 背景の黒い透明なボックスを描画
                SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
                DrawBox(280, 580, 1000, 720, GetColor(0, 0, 0), TRUE);
                SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

                // 外枠（白）
                DrawBox(280, 580, 1000, 720, GetColor(255, 255, 255), FALSE);

                std::string text = "";
                int currentFaceHandle = -1;

                // プロローグ終了後のプレイヤーのセリフ
                if (isFirstPrologueEnd)
                {
                    text = "「ここはどこだ？！」";
                    currentFaceHandle = hPlayerFace; // プレイヤーのアイコン
                }
                //NPCのセリフ
                else
                {
                    float dx = (player.GetX() + 64) - (npc.GetX() + 64);
                    float dy = (player.GetY() + 120) - (npc.GetY() + 120);
                    float distanceSq = (dx * dx) + (dy * dy);

                    if (distanceSq <= (130.0f * 130.0f))
                    {
                        text = npc.GetMessage();
                        currentFaceHandle = hNPCFace;
                    }
                    else
                    {
                        text = "「……。」";
                        currentFaceHandle = hPlayerFace;
                    }
                }

                // ③ 選ばれた顔グラフィックと文字列を描画
                if (currentFaceHandle != -1)
                {
                    DrawExtendGraph(290, 590, 290 + 120, 590 + 120, currentFaceHandle, TRUE);
                }

                SetFontSize(24);
                DrawFormatString(430, 600, GetColor( 255, 255, 255), "%s", text.c_str());
                SetFontSize(16);
            }
            else
            {
                if (hasTalkedFirst)
                {
                    // 会話ウィンドウが閉じられたら、プロローグ直後フラグを安全に解除して通常モードに戻す
                    isFirstPrologueEnd = false;
                }
            }
        }

        ScreenFlip();
    }

    DeleteGraph(hPlayerFace);
    DeleteGraph(hNPCFace);
    DxLib_End();
    return 0;
}
