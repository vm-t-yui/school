#include "DxLib.h"
#include "Camera.h"

#define _USE_MATH_DEFINES
#include <math.h>

/// <summary>
/// コンストラクタ
/// </summary>
Camera::Camera()
	: fov		(60.0f)
	 , pos		(VGet(0,0,0))
{
	//奥行0.1f～1000.0fまでをカメラの描画範囲とする
	SetCameraNearFar(0.1f, 1000.0f);
}

/// <summary>
/// 更新
/// </summary>
void Camera::Update()
{
	// FOV(視野角)
	SetupCamera_Perspective(fov * (static_cast<float>(M_PI) / 180.0f));

	// とりあえず原点から雑に離れた位置で原点を見る
	pos = VGet(0, 10.0f, -20.0f);
	SetCameraPositionAndTarget_UpVecY(pos, VGet(0, pos.y, 0));
}