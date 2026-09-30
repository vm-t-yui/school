// 2026 Takeru Yui All rights reserved.
#pragma once

// ============================================================================
// Animation
//
// 3Dモデルに含まれるアニメーションを扱うための実装例
// 初期テンプレートでは使用しない
// Animationは状態を持つ部品なので、独立して制御したいモデルごとに用意する
// この実装は60FPS固定を前提に、1回のUpdateで1フレーム分進める
// より高度な制御が必要ならゲームに合わせて自由に拡張してよい
// ============================================================================
namespace System
{
    class Animation
    {
    public:
        /// <summary>
        /// モデルに含まれるアニメーションを開始したい瞬間に1回だけ再生する
        /// </summary>
        void Play(int modelHandle, int animationIndex, bool isLoop);

        /// <summary>
        /// 再生中のアニメーションを停止する
        /// </summary>
        void Stop();

        /// <summary>
        /// 1フレームごとに進めるアニメーション時間を設定する
        /// </summary>
        void SetSpeed(float speedPerFrame);

        /// <summary>
        /// アニメーションを再生中か確認する
        /// </summary>
        bool IsPlaying() const;

        /// <summary>
        /// ループしないアニメーションが最後まで再生されたか確認する
        /// </summary>
        bool IsFinished() const;

        /// <summary>
        /// 再生中のアニメーションを1フレーム更新する
        /// </summary>
        void Update();

    private:
        int   modelHandle   = -1;
        int   attachIndex   = -1;
        float totalTime     = 0.0f;
        float currentTime   = 0.0f;
        float speedPerFrame = 1.0f;
        bool  isLooping     = false;
        bool  isPlaying     = false;
        bool  isFinished    = false;
    };
}
