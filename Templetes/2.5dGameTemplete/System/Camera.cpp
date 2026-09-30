// 2026 Takeru Yui All rights reserved.
#include "Camera.h"
#include "SystemConfig.h"

namespace System
{
    /// <summary>
    /// カメラの基準位置を設定する
    /// </summary>
    void Camera::SetPosition(const VECTOR& newPosition)
    {
        position = newPosition;
    }

    /// <summary>
    /// カメラが見る基準位置を設定する
    /// </summary>
    void Camera::SetTarget(const VECTOR& newTarget)
    {
        target = newTarget;
    }

    /// <summary>
    /// カメラの視野角を度数法で設定する
    /// </summary>
    void Camera::SetFieldOfView(float degree)
    {
#if SYSTEM_ENABLE_DEBUG_CHECK
        if (degree <= 0.0f || degree >= 180.0f)
        {
            printfDx("[System::Camera] Unusual field of view: %.2f\n", degree);
        }
#endif
        fieldOfViewDegree = degree;
    }

    /// <summary>
    /// カメラが描画する手前と奥の距離を設定する
    /// </summary>
    void Camera::SetNearFar(float newNearDistance, float newFarDistance)
    {
#if SYSTEM_ENABLE_DEBUG_CHECK
        if (newNearDistance <= 0.0f || newFarDistance <= newNearDistance)
        {
            printfDx(
                "[System::Camera] Invalid near/far: %.2f / %.2f\n",
                newNearDistance,
                newFarDistance);
        }
#endif
        nearDistance = newNearDistance;
        farDistance  = newFarDistance;
    }

    /// <summary>
    /// カメラ位置へ一時的に加えるオフセットを設定する
    /// </summary>
    void Camera::SetPositionOffset(const VECTOR& offset)
    {
        positionOffset = offset;
    }

    /// <summary>
    /// カメラの注視点へ一時的に加えるオフセットを設定する
    /// </summary>
    void Camera::SetTargetOffset(const VECTOR& offset)
    {
        targetOffset = offset;
    }

    /// <summary>
    /// カメラの一時的なオフセットをリセットする
    /// </summary>
    void Camera::ResetOffset()
    {
        positionOffset = VGet(0.0f, 0.0f, 0.0f);
        targetOffset   = VGet(0.0f, 0.0f, 0.0f);
    }

    /// <summary>
    /// 保存しているカメラ設定をDxLibへ反映する
    /// </summary>
    void Camera::Apply() const
    {
        const float radian         = fieldOfViewDegree * (DX_PI_F / 180.0f);
        const VECTOR finalPosition = VAdd(position, positionOffset);
        const VECTOR finalTarget   = VAdd(target, targetOffset);

        SetupCamera_Perspective(radian);
        SetCameraNearFar(nearDistance, farDistance);
        SetCameraPositionAndTarget_UpVecY(finalPosition, finalTarget);
    }
}
