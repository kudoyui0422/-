#include "Prologue.h"


Prologue::Prologue()
{
    // トラックのキキー音を読み込み
    sfxTruck = LoadSoundMem("音/truck.mp3");
    prologueStep = 0;
    oldKeyReturn = false;
}

Prologue::~Prologue()
{
    // 音声データの解放
    DeleteSoundMem(sfxTruck);
}

int Prologue::Update()
{
    // 毎フレームのエンターキーの状態を取得
    bool currentKeyReturn = (CheckHitKey(KEY_INPUT_RETURN) == 1);
    int result = 0; // 0: 継続

    if (prologueStep == 0)
    {
        // 「エンターキーが今押されたら」次のステップへ
        if (currentKeyReturn && !oldKeyReturn)
        {
            prologueStep = 1;
            // トラックの音を再生
            PlaySoundMem(sfxTruck, DX_PLAYTYPE_BACK);
        }
    }
    else if (prologueStep == 1)
    {
        // 音が鳴り終わるか、再度エンターが押されたら終了フラグ(1)を返す
        if (CheckSoundMem(sfxTruck) == 0 || (currentKeyReturn && !oldKeyReturn))
        {
            result = 1; // 1: プロローグ終了（本編へ）
        }
    }

    oldKeyReturn = currentKeyReturn;
    return result;
}

void Prologue::Draw()
{
    // 背景は main.cpp の ClearDrawScreen() で黒くなっているので、文字だけ描画する
    if (prologueStep == 0)
    {
        SetFontSize(32);
        DrawString(400, 330, "ある日僕は事故にあった", GetColor(255, 255, 255));
        SetFontSize(16);
    }
}

