// 絶望☆異世界転移.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//
#include<DxLib.h>
#include <iostream>
#include "Screen.h"
#include"Player.h"
#include"Field.h"
#include"NPC.h"

int main()
{
    //コンソールでDXライブラリを使う時は最初にこれを入れる
    ChangeWindowMode(TRUE);

    //画面サイズ
    SetGraphMode(Screen::WIDTH, Screen::HEIGHT, 32);
    SetOutApplicationLogValidFlag(FALSE); //ログを出さない

    SetMainWindowText(Screen::WINDOW_NAME);
    SetWindowSizeExtendRate(Screen::WINDOW_EXTEND);
    ChangeWindowMode(Screen::WINDOW_MODE); //Windowモードの場合

    //DXライブラリの初期化処理
    if (DxLib_Init() == -1)
    {
        return -1; //初期化に失敗したら終了
    }
    
    //画面を裏画面（描画専用のバッファ）に設定（画面のちらつきを防ぐため）
    SetDrawScreen(DX_SCREEN_BACK);

    //プレイヤーとフィールドの変数（インスタンス）を作る
    Player player;
    Field field;
    NPC npc;
    player.SetField(&field);
    npc.SetField(&field);

    //プレイヤーにフィールドの場所を教えてあげる
    player.SetField(&field);
    npc.SetField(&field);

    //メインループ（Escキーが押されるか、ウィンドウが閉じられるまで繰り返す）
    while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
    {
        //裏画面の描画をクリア
        ClearDrawScreen();

        //ゲームの処理や描画関数（DrawGraph,DrawBoxなど）

        player.Update(npc);  //キー入力の受付や、フィールド都の当たり判定を計算

        field.Draw(); //背景や床を先に描画する

        //プレイヤーとNPCのY座標を比較して、上にいる（Y座標が小さい）キャラから先に描画
        if (player.GetY() < npc.GetY())
        {
            //プレイヤーの方が上にいるから、先にプレイヤーで、手前にNPC
            player.Draw();
            npc.Draw();
        }
        else
        {
            //NPCの方が上にいるから、先にNPCで、手前にプレイヤー
            npc.Draw();
            player.Draw();
        }
     

        //裏画面の内容は実際の画面に反映（フリップ）
        ScreenFlip();
    }

    //DXライブラリの後始末
    DxLib_End(); 

    return 0;
}


