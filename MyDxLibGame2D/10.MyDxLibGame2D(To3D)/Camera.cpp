// 2016 Takeru Yui All Rights Reserved.
#include "DxLib.h"
#include <vector>
#include "Camera.h"

/// <summary>
/// コンストラクタ
/// </summary>
Camera::Camera()
{
	//奥行0.1～1000までをカメラの描画範囲とする
	SetCameraNearFar(0.1f, 1000.0f);

	// 透視投影のカメラをセットアップする（視野角の設定）
	float fovDegree = 60.0f;
	float fovRadian = fovDegree * (180.0f / DX_PI_F);
	SetupCamera_Perspective(fovRadian);	// ラジアンに変換して使う

	// ポジションとターゲットからカメラの回転とポジションを同時設定する
	VECTOR pos		= VGet(0, 0, -5);
	VECTOR target	= VGet(0, 0, 0);
	SetCameraPositionAndTarget_UpVecY(pos, target);
}

/// <summary>
/// デストラクタ
/// </summary>
Camera::~Camera()
{
	// 処理なし
}