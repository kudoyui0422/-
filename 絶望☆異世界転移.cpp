// 絶望☆異世界転移.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//
#include"DxLib.h"
#include"Player.h"
#include <iostream>

int main()
{
    //画面モードの変更（ウィンドモードにする）
    ChangeWindowMode(TRUE);

    //DXライブラリの初期化
    if (DxLib_Init() == -1) return -1;

    //描画先を裏画面に設定（これをしないとScreenFlipが効かない）
    SetDrawScreen(DX_SCREEN_BACK);

    //プレイヤーの生成
    Player player;

    //ゲームループ（ウィンドウが閉じられるか。ESCキーが押されるまでループ）
    while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
    {
        //画面をクリア
        ClearDrawScreen();

        //プレイヤーの更新と描画
        player.Update();
        player.Draw();

        //裏画面の内容を表画面に反映
        ScreenFlip();
    }

    //DXライブラリの後始末
    DxLib_End();

    return 0;
}
