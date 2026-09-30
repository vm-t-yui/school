// 2026 Takeru Yui All rights reserved.
#pragma once

#include "DxLib.h"

// ============================================================================
// Camera
//
// DxLibの3Dカメラを扱うための実装例
// 疑似2D用のFOVや位置はBeginner2D3D側が初期値として設定する
// Cameraは状態を持つ部品なので、利用側がインスタンスを所有する
// 各SetterはCameraが保持する値だけを変更し、DxLibへは自動反映しない
// 複数作成した場合はApplyを呼んだCameraの設定が実際の描画に使われる
// この実装をそのまま使う必要はなく、ゲームに合わせて変更・置換してよい
// ============================================================================
namespace System
{
    class Camera
    {
    public:
        /// <summary>
        /// カメラの基準位置を設定する
        /// </summary>
        void SetPosition(const VECTOR& position);

        /// <summary>
        /// カメラが見る基準位置を設定する
        /// </summary>
        void SetTarget(const VECTOR& target);

        /// <summary>
        /// カメラの視野角を度数法で設定する
        /// </summary>
        void SetFieldOfView(float degree);

        /// <summary>
        /// カメラが描画する手前と奥の距離を設定する
        /// </summary>
        void SetNearFar(float nearDistance, float farDistance);

        /// <summary>
        /// カメラ位置へ一時的に加えるオフセットを設定する
        /// </summary>
        void SetPositionOffset(const VECTOR& offset);

        /// <summary>
        /// カメラの注視点へ一時的に加えるオフセットを設定する
        /// </summary>
        void SetTargetOffset(const VECTOR& offset);

        /// <summary>
        /// カメラの一時的なオフセットをリセットする
        /// </summary>
        void ResetOffset();

        /// <summary>
        /// 保存しているカメラ設定をDxLibへ反映する
        /// </summary>
        void Apply() const;

    private:
        VECTOR position          = VGet(0.0f, 0.0f, -10.0f);
        VECTOR target            = VGet(0.0f, 0.0f, 0.0f);
        VECTOR positionOffset    = VGet(0.0f, 0.0f, 0.0f);
        VECTOR targetOffset      = VGet(0.0f, 0.0f, 0.0f);
        float  fieldOfViewDegree = 60.0f;
        float  nearDistance      = 0.1f;
        float  farDistance       = 1000.0f;
    };
}
