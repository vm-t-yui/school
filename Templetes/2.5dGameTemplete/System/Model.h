// 2026 Takeru Yui All rights reserved.
#pragma once

#include "DxLib.h"

// ============================================================================
// Model
//
// DxLibの3Dモデル操作を小さな単位でまとめた実装例
// この実装をそのまま使う必要はなく、ゲームに合わせて変更・置換してよい
// Beginner2D3D固有の座標系や描画ルールはここでは扱わない
// ============================================================================
namespace System::Model
{
    /// <summary>
    /// 3Dモデルを読み込む
    /// </summary>
    int Load(const char* filePath);

    /// <summary>
    /// 読み込み済みの3Dモデルを、個別の状態を持てる別モデルとして複製する
    /// </summary>
    int Duplicate(int modelHandle);

    /// <summary>
    /// 3Dモデルを削除する
    /// </summary>
    void Delete(int modelHandle);

    /// <summary>
    /// 3Dモデルの位置を設定する
    /// </summary>
    void SetPosition(int modelHandle, const VECTOR& position);

    /// <summary>
    /// 3Dモデルの回転をラジアンで設定する
    /// </summary>
    void SetRotation(int modelHandle, const VECTOR& rotationRadian);

    /// <summary>
    /// 3Dモデルのスケールを設定する
    /// </summary>
    void SetScale(int modelHandle, const VECTOR& scale);

    /// <summary>
    /// 3Dモデルを描画する
    /// </summary>
    void Draw(int modelHandle);
}
