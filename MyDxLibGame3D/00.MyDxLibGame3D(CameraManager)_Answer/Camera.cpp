// 2016 Takeru Yui All Rights Reserved.
#include "DxLib.h"
#include <vector>
#include "GlobalConstants.h"
#include "VirtualCameraManager.h"
#include "Camera.h"

namespace
{
	// near,farはなるべく小さくする
	const static float CameraNear	= 0.5f;		// near
	const static float CameraFar	= 1000.0f;	// far

	// カメラのポジションは原点からZ-方向に少し離れたところ
	// ターゲットは原点
	const static VECTOR CameraFirstPos		= VGet(0.0f, 5.0f, -10.0f);
	const static VECTOR CameraFirstTarget	= VGet(0.0f, 0.0f, 0.0f);
}

/// <summary>
/// コンストラクタ
/// </summary>
Camera::Camera()
{
	//奥行0.1～1000までをカメラの描画範囲とする
	SetCameraNearFar(CameraNear, CameraFar);
}

/// <summary>
/// デストラクタ
/// </summary>
Camera::~Camera()
{
	// 処理なし
}

/// <summary>
/// カメラの更新
/// </summary>
void Camera::Update(const VirtualCameraManager& virtualCameraManager)
{
	// 透視投影のカメラをセットアップする（視野角の設定）
	float fovRadian = virtualCameraManager.GetResultFovDegree() * (DX_PI_F / 180.0f);
	SetupCamera_Perspective(fovRadian);	// ラジアンに変換して使う

	// ポジションとターゲットからカメラの回転とポジションを同時設定する
	// カメラのターゲットは３Dスクリーンの中心
	VECTOR pos = virtualCameraManager.GetResultPos();
	VECTOR target = virtualCameraManager.GetResultTarget();
	SetCameraPositionAndTarget_UpVecY(pos, target);
}