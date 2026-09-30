// 2026 Takeru Yui All rights reserved.
#include "DxLib.h"
#include "Animation.h"
#include "SystemConfig.h"

namespace System
{
    /// <summary>
    /// モデルに含まれるアニメーションを開始したい瞬間に1回だけ再生する
    /// </summary>
    void Animation::Play(int newModelHandle, int animationIndex, bool isLoop)
    {
        if (newModelHandle < 0)
        {
#if SYSTEM_ENABLE_DEBUG_CHECK
            printfDx("[System::Animation] Invalid model handle: %d\n", newModelHandle);
#endif
            return;
        }

        // 同じAnimationオブジェクトで別の再生を始める場合は、先に現在の再生を外す
        if (attachIndex >= 0 && modelHandle >= 0)
        {
            MV1DetachAnim(modelHandle, attachIndex);
        }

        modelHandle = newModelHandle;
        attachIndex = MV1AttachAnim(modelHandle, animationIndex, -1, FALSE);

        if (attachIndex < 0)
        {
#if SYSTEM_ENABLE_DEBUG_CHECK
            printfDx(
                "[System::Animation] Failed to attach animation: model=%d animation=%d\n",
                modelHandle,
                animationIndex);
#endif
            modelHandle = -1;
            attachIndex = -1;
            totalTime   = 0.0f;
            currentTime = 0.0f;
            isLooping   = false;
            isPlaying   = false;
            isFinished  = false;
            return;
        }

        totalTime   = MV1GetAttachAnimTotalTime(modelHandle, attachIndex);
        currentTime = 0.0f;
        isLooping   = isLoop;
        isPlaying   = true;
        isFinished  = false;

        MV1SetAttachAnimTime(modelHandle, attachIndex, currentTime);
    }

    /// <summary>
    /// 再生中のアニメーションを停止する
    /// </summary>
    void Animation::Stop()
    {
        if (attachIndex >= 0 && modelHandle >= 0)
        {
            MV1DetachAnim(modelHandle, attachIndex);
        }

        modelHandle = -1;
        attachIndex = -1;
        totalTime   = 0.0f;
        currentTime = 0.0f;
        isLooping   = false;
        isPlaying   = false;
        isFinished  = false;
    }

    /// <summary>
    /// 1フレームごとに進めるアニメーション時間を設定する
    /// </summary>
    void Animation::SetSpeed(float newSpeedPerFrame)
    {
        speedPerFrame = newSpeedPerFrame;
    }

    /// <summary>
    /// アニメーションを再生中か確認する
    /// </summary>
    bool Animation::IsPlaying() const
    {
        return isPlaying;
    }

    /// <summary>
    /// ループしないアニメーションが最後まで再生されたか確認する
    /// </summary>
    bool Animation::IsFinished() const
    {
        return isFinished;
    }

    /// <summary>
    /// 再生中のアニメーションを1フレーム更新する
    /// </summary>
    void Animation::Update()
    {
        if (!isPlaying || modelHandle < 0 || attachIndex < 0)
        {
            return;
        }

        currentTime += speedPerFrame;

        if (isLooping)
        {
            if (totalTime > 0.0f && currentTime >= totalTime)
            {
                currentTime = 0.0f;
            }
        }
        else if (currentTime >= totalTime)
        {
            currentTime = totalTime;
            isPlaying   = false;
            isFinished  = true;
        }

        MV1SetAttachAnimTime(modelHandle, attachIndex, currentTime);
    }
}
