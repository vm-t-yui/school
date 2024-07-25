// 2024 Takeru Yui All Rights Reserved.
#include "DxLib.h"
#include <string>
#include "Screen.h"
#include "YuiLib.h"
#include "Player.h"

/// <summary>
/// コンストラクタ
/// </summary>
Player::Player()
    : modelHandle               (-1)
    , animHandle                { -1 }
    , currentAnimTotalTime      (0)
    , currentAnimAttachIndex    (-1)
    , currentAnimTime           (0)
    , currentAnim               (AnimKind::Num)
{
    // ３Ｄモデルの読み込み
    modelHandle = MV1LoadModel("data/SwapModel.mv1");

    // アニメーションの読み込み
    animHandle[(int)AnimKind::Idle] = MV1LoadModel("data/SwapIdlemv1.mv1");
    animHandle[(int)AnimKind::Run] = MV1LoadModel("data/AnimRun.mv1");

    SetAnimation(AnimKind::Idle, 0);

    // 画面に映る位置に３Ｄモデルを移動
    MV1SetPosition(modelHandle, VGet(0, 0, 0));
    MV1SetScale(modelHandle, VGet(0.1f, 0.1f, 0.1f));
}

/// <summary>
/// 更新
/// </summary>
void Player::Update()
{
    int Key = GetJoypadInputState(DX_INPUT_KEY_PAD1);
    if (Key & PAD_INPUT_RIGHT && currentAnim != AnimKind::Idle)
    {
        SetAnimation(AnimKind::Idle, 0);
    }
    else if (Key & PAD_INPUT_LEFT && currentAnim != AnimKind::Run)
    {
        SetAnimation(AnimKind::Run, 1);
    }

    // 再生時間を進める
    // 元のアニメーションが30fpsなので、60fpsのこのサンプルでは0.5f進める
    currentAnimTime += 0.5f;

    // 再生時間がアニメーションの総再生時間に達したら再生時間を０に戻す
    if (currentAnimTime >= currentAnimTotalTime)
    {
        currentAnimTime = 0.0f;
    }

    // 再生時間をセットする
    MV1SetAttachAnimTime(modelHandle, currentAnimAttachIndex, currentAnimTime);
}

/// <summary>
/// 描画
/// </summary>
void Player::Draw()
{
    // ３Ｄモデルの描画
    MV1DrawModel(modelHandle);
}

/// <summary>
/// アニメーションセット
/// </summary>
void Player::SetAnimation(AnimKind kind, int animIndex)
{
    currentAnim = kind;

    // TODO: ブレンド

    // 再生しているものがあればでタッチする
    if (currentAnimAttachIndex >= 0)
    {
         MV1DetachAnim(modelHandle, currentAnimAttachIndex);
    }
    
    // アニメーションをアタッチする
    currentAnimAttachIndex = MV1AttachAnim(modelHandle, animIndex, animHandle[static_cast<int>(kind)], TRUE);

    // アタッチしたアニメーションの総再生時間を取得する
    currentAnimTotalTime = MV1GetAttachAnimTotalTime(modelHandle, currentAnimAttachIndex);

    // 再生時間の初期化
    currentAnimTime = 0.0f;
}