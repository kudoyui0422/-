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
    STATE_GAME,
    STATE_ENDING
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

    // --- エンディング画像用のハンドル ---
    int hHappyEndGraph = LoadGraph("イラスト/ハッピーエンド.jpg");  // エンド1用
    int hBadEndGraph = LoadGraph("イラスト/バッドエンド.jpg");  // エンド2用
    int hNormalEndGraph = LoadGraph("イラスト/ノーマルエンド.jpg"); // ノーマル用
    int hEvolutionEndGraph = -1; // エンド3用（まだ無いのでとりあえず-1）
    int hWeakEndGraph = -1; // エンド4用（まだ無いのでとりあえず-1）

    // プロローグ終了後のウェイト用タイマー（60フレーム ＝ 約1秒）

    int prologueWaitTimer = 0;

    bool isFirstPrologueEnd = false;
    bool hasStartedFirstTalk = false; //独り言が始まったか
    bool hasTalkedFirst = false; //独り言が終わったか

    //選択肢・分岐・エンドの管理用変数

    int questionIndex = 0;        //0:質問前、1～4:書く質問の進行度
    bool isSelecting = false;     //選択肢を表示中か
    int selectCursor = 0;         //選択肢のカーソル（0:正解, 1:不正解）
    bool isNPCAnswering = false;  //選択肢を選んだ後のNPCのリアクションを表示中か
    bool playerAnswers[4] = { false, false, false, false }; //プレイヤーの選択（true:正解、false:不正解）

    int currentEndType = 0;       //1:ハッピー, 2:バッド, 3:進化, 4:衰弱, 5:ノーマル
    int hCurrentEndGraph = -1;    //現在表示するエンディング画像のハンドル


    // main側で会話中のエンターキーの「押し下がり」を管理するフラグ
    bool oldMainKeyEnter = false;
    GameState state = STATE_TITLE;


    while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
    {
        ClearDrawScreen();

        //現在のフレームのエンターキーの入力状態を取得
        bool currentKeyEnter = (CheckHitKey(KEY_INPUT_RETURN) == 1);

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

        // ② プロローグ画面の処理

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
                hasTalkedFirst = false; //プロローグに入るたびにフラグを初期化
                hasStartedFirstTalk = false; //初期化
                npc.ResetMessage(); //NPCのメッセージも初期化
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
                    hasStartedFirstTalk = true;
                }
            }

            //会話中でないときだけプレイヤーの移動開始チェック
            if (!player.GetIsTalking())
            {
                player.Update(npc);
            }

            // 【ロジック一括処理】エンターキーによる会話・質問の進行判定
            if (player.GetIsTalking() && currentKeyEnter && !oldMainKeyEnter)
            {
                if (isFirstPrologueEnd && !hasTalkedFirst)
                {
                    // プロローグ直後の独り言を終了する
                    player.SetIsTalking(false);
                }
                else
                {
                    float dx = (player.GetX() + 64) - (npc.GetX() + 64);
                    float dy = (player.GetY() + 120) - (npc.GetY() + 120);
                    float distanceSq = (dx * dx) + (dy * dy);

                    if (distanceSq <= (130.0f * 130.0f))
                    {
                        // 通常セリフ中（質問が始まる前）
                        if (questionIndex == 0)
                        {
                            npc.AdvanceMessage();
                            if (npc.GetMessageIndex() >= 4)
                            {
                                questionIndex = 1; // NPCの最初のセリフ（0~3）が終わったら質問1へ移行
                                isSelecting = false;
                                isNPCAnswering = false;
                            }
                        }
                        // 質問文が表示されている状態の時
                        else if (questionIndex >= 1 && questionIndex <= 4 && !isSelecting && !isNPCAnswering)
                        {
                            isSelecting = true;
                            selectCursor = 0; // カーソルを初期化
                        }
                        // 選択肢を選び、エンターを押した時
                        else if (isSelecting)
                        {
                            playerAnswers[questionIndex - 1] = (selectCursor == 0); // 上（0）なら正解(A), 下（1）なら不正解(B)
                            isSelecting = false;
                            isNPCAnswering = true; // NPCのリアクションへ移行
                        }

                        // NPCの反応が表示されている時
                        else if (isNPCAnswering)
                        {
                            isNPCAnswering = false;
                            questionIndex++; // 次の質問へ進む

                            // すべての質問（4つ）が終わったらエンディングの判定
                            if (questionIndex > 4)
                            {
                                player.SetIsTalking(false);
                                npc.ResetMessage();
                                questionIndex = 0;

                                // --- 画像に合わせたエンド分岐判定 ---
                                bool q1 = playerAnswers[0];
                                bool q2 = playerAnswers[1];
                                bool q3 = playerAnswers[2];
                                bool q4 = playerAnswers[3];

                                if (q1 && q2 && q3 && q4) 
                                {
                                    currentEndType = 1;                // ハッピーエンド
                                    hCurrentEndGraph = hHappyEndGraph;
                                }
                                else if (!q1 && !q2 && !q3 && q4) {
                                    currentEndType = 2;               // バッドエンド 
                                    hCurrentEndGraph = hBadEndGraph;
                                }
                                else if (q1 && !q2 && !q3 && !q4)
                                {
                                    currentEndType = 3;              // 進化エンド
                                    hCurrentEndGraph = hEvolutionEndGraph;
                                }
                                else if (!q1 && q2 && q3 && q4) 
                                {
                                    currentEndType = 4;              // 衰弱エンド
                                    hCurrentEndGraph = hWeakEndGraph;
                                }
                                else
                                {
                                    currentEndType = 5;              // ノーマルエンド
                                    hCurrentEndGraph = hNormalEndGraph;
                                }

                                // エンディング画面状態へ切り替え
                                state = STATE_ENDING;
                            }
                        }
                    }
                    else
                    {
                        player.SetIsTalking(false);
                    }
                }
            }


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
                std::string optionA = ""; // 選択肢A（正解用）
                std::string optionB = ""; // 選択肢B（不正解用）
                int currentFaceHandle = -1;

                // プロローグ終了後のプレイヤーのセリフ
                if (isFirstPrologueEnd && !hasTalkedFirst)
                {
                    text = "「ここはどこだ？！」";
                    currentFaceHandle = hPlayerFace; // プレイヤーのアイコン

                    // 独り言の最中にエンターキーが押されたら会話を閉じる
                    if (currentKeyEnter && !oldMainKeyEnter)
                    {
                        player.SetIsTalking(false);
                    }
                }


                // NPCのセリフと選択肢
                else
                {
                    float dx = (player.GetX() + 64) - (npc.GetX() + 64);
                    float dy = (player.GetY() + 120) - (npc.GetY() + 120);
                    float distanceSq = (dx * dx) + (dy * dy);

                    if (distanceSq <= (130.0f * 130.0f))
                    {
                        //質問フェーズがまだ始まっていないとき
                        if (questionIndex == 0)
                        {
                            text = npc.GetMessage();

                            // 2ページ目「どうやって帰るの？」の時だけ主人公の顔にする
                            if (npc.GetMessageIndex() == 2)
                            {
                                currentFaceHandle = hPlayerFace;
                            }
                            else
                            {
                                currentFaceHandle = hNPCFace;
                            }
                        }

                        //質問フェーズ（質問は１～４）
                        else if (questionIndex >= 1 && questionIndex <= 4)
                        {
                            currentFaceHandle = hNPCFace; //質問中は基本的にNPCの顔

                            //NPCの質問文を表示中
                            if (!isSelecting && !isNPCAnswering)
                            {
                                if (questionIndex == 1) text = "NPC「僕のことを見てどう思った？」";
                                if (questionIndex == 2) text = "NPC「君のいた世界はどんな感じだった？」";
                                if (questionIndex == 3) text = "NPC「君は強くなりたいと思う？」";
                                if (questionIndex == 4) text = "NPC「最後の質問、元の世界に帰りたい？」";
                            }

                            // プレイヤーの選択肢を選び中
                            else if (isSelecting)
                            {
                                currentFaceHandle = hPlayerFace; //主人公の顔

                                if (questionIndex == 1) { text = "（どう答えよう…）"; optionA = "つのと翼がかっこいいなとおもった！"; optionB = "なんかコスプレした変な奴がいるなと思った。"; }
                                if (questionIndex == 2) { text = "（元の世界は…）"; optionA = "嫌なこともあるけど楽しい世界だよ。"; optionB = "お前に関係ないだろ"; }
                                if (questionIndex == 3) { text = "（強くなりたい…？）"; optionA = "うーん あんまり思わないかな。"; optionB = "強くなりたい。"; }
                                if (questionIndex == 4) { text = "（帰りたいか…）"; optionA = "帰りたい！"; optionB = "……帰りたくない"; }

                                // 上下キーでカーソル移動
                                if (CheckHitKey(KEY_INPUT_UP) == 1 || CheckHitKey(KEY_INPUT_DOWN) == 1)
                                {
                                    // 押しっぱなし防止のため簡易的に反転（今回のフレームで切り替え）
                                    if (GetRand(10) > 8) {} // 押しっぱなし制御
                                }
                                //キーの上下移動
                                static bool oldUpDown = false;
                                bool currentUp = (CheckHitKey(KEY_INPUT_UP) == 1);
                                bool currentDown = (CheckHitKey(KEY_INPUT_DOWN) == 1);
                                if ((currentUp || currentDown) && !oldUpDown) {
                                    selectCursor = (selectCursor == 0) ? 1 : 0;
                                }
                                oldUpDown = (currentUp || currentDown);
                            }

                            // NPCのリアクションを表示中
                            else if (isNPCAnswering)
                            {
                                bool isCorrect = playerAnswers[questionIndex - 1];
                                if (questionIndex == 1) text = isCorrect ? "NPC「えへへ ありがとー」" : "NPC「………………」";
                                if (questionIndex == 2) text = isCorrect ? "NPC「なるほどねー」" : "NPC「……まぁそうだね」";
                                if (questionIndex == 3) text = isCorrect ? "NPC「えー？そうなんだー」" : "NPC「うんうん！強くなりたいよね」";
                                if (questionIndex == 4) text = isCorrect ? "NPC「ふーん」" : "NPC「……そっかぁ」";
                            }
                        }

                    }
                    else
                    {
                        text = "「……。」";
                        currentFaceHandle = hPlayerFace;
                    }
                }

                // ③ 選ばれた顔グラフィックと文字列を描画
                if (player.GetIsTalking())
                {
                    if (currentFaceHandle != -1)
                    {
                        DrawExtendGraph(290, 590, 290 + 120, 590 + 120, currentFaceHandle, TRUE);
                    }

                    SetFontSize(24);
                    DrawFormatString(430, 600, GetColor(255, 255, 255), "%s", text.c_str());

                    // 【追加】選択肢画面中（isSelecting == true）であれば、選択肢テキストとカーソルを描画する
                    if (isSelecting)
                    {
                        int cursorY = (selectCursor == 0) ? 645 : 680;
                        DrawFormatString(450, 645, (selectCursor == 0) ? GetColor(255, 215, 0) : GetColor(180, 180, 180), "%s", optionA.c_str());
                        DrawFormatString(450, 680, (selectCursor == 1) ? GetColor(255, 215, 0) : GetColor(180, 180, 180), "%s", optionB.c_str());
                        DrawFormatString(425, cursorY, GetColor(255, 215, 0), "▶");
                    }

                    SetFontSize(16);
                }
            }

            else
            {
                //会話中でない時の処理
                if (isFirstPrologueEnd && hasStartedFirstTalk)
                {
                    // 会話ウィンドウが閉じられたら、プロローグ直後フラグを安全に解除して通常モードに戻す
                    hasTalkedFirst = true;
                    isFirstPrologueEnd = false;
                }
            }
        }

        // ④ 【新規追加】ゲーム終了後のエンディング画面（STATE_ENDING）の描画・更新処理
    else if (state == STATE_ENDING)
    {
        // エンディング画像が読み込まれていれば全画面に描画
        if (hCurrentEndGraph != -1)
        {
            DrawExtendGraph(0, 0, Screen::WIDTH, Screen::HEIGHT, hCurrentEndGraph, FALSE);
        }
        else
        {
            // まだイラストが無い（-1の）場合は黒背景にテキストを表示して対応（エンド3,4用）
            DrawBox(0, 0, Screen::WIDTH, Screen::HEIGHT, GetColor(0, 0, 0), TRUE);
            SetFontSize(40);
            DrawFormatString(100, 200, GetColor(255, 255, 255), "イラスト制作中...");
            SetFontSize(16);
        }

        // 左下に現在のエンディング名を表示する
        SetFontSize(32);
        std::string endName = "";
        unsigned int textColor = GetColor(255, 255, 255); //通常の文字の色は白

        if (currentEndType == 1) endName = "END 1 「ハッピーエンド」";

        if (currentEndType == 2)
        {
            endName = "「バッドエンド」";
            //バッドエンドの時は文字は赤
            textColor = GetColor(200, 0, 0);
        }
        
        if (currentEndType == 3) endName = "END 3 「進化エンド」";
        if (currentEndType == 4) endName = "END 4 「衰弱エンド」";
        if (currentEndType == 5) endName = "その他 「ノーマルエンド」";
        DrawFormatString(60, Screen::HEIGHT - 100, textColor, "%s", endName.c_str());
        SetFontSize(16);

        // エンディング画面でエンターキーを押したらタイトルに戻る
        if (currentKeyEnter && !oldMainKeyEnter)
        {
            state = STATE_TITLE;
            SetMouseDispFlag(TRUE); // タイトルに戻るのでマウスを再表示
        }
    }

            // フレームの入力状態を保存してループの先頭に戻る
            oldMainKeyEnter = currentKeyEnter;

            ScreenFlip();
    }

    ::DeleteGraph(hPlayerFace);
    ::DeleteGraph(hNPCFace);

    // 【追加】エンディング画像のメモリを安全に解放
    if (hHappyEndGraph != -1)  ::DeleteGraph(hHappyEndGraph);
    if (hBadEndGraph != -1)    ::DeleteGraph(hBadEndGraph);
    if (hNormalEndGraph != -1) ::DeleteGraph(hNormalEndGraph);
    if (hEvolutionEndGraph != -1) ::DeleteGraph(hEvolutionEndGraph);
    if (hWeakEndGraph != -1)   ::DeleteGraph(hWeakEndGraph);

    ::DxLib_End();
    return 0;

}