//-----------------------------------------------------------------------------
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#include "DxLib.h"
#include "Player.h"
#include "Camera.h"

#define _USE_MATH_DEFINES
#include <math.h>

#define USE_LERP_CAMERA 1

/// <summary>
/// コンストラクタ
/// </summary>
Camera::Camera()
	: pos(VGet(0, 0, 0))
{
	//奥行0.1～1000までをカメラの描画範囲とする
	SetCameraNearFar(0.1f, 1000.0f);

	// FOV(視野角)を60度に
	SetupCamera_Perspective(60.0f * (static_cast<float>(M_PI) / 180.0f));
}

/// <summary>
/// デストラクタ
/// </summary>
Camera::~Camera()
{
	// 処理なし
}

/// <summary>
/// 更新
/// </summary>
void Camera::Update(const Player& player)
{
#if !USE_LERP_CAMERA
	// z軸上で、プレイヤーから一定距離離れた状態でプレイヤーを常に見続けるよう位置調整
	pos = VGet(0, player.GetPos().y + 20.0f, player.GetPos().z - 30.0f);
#else
	// lerpを使用して実装.
	// lerp(VECTOR a, VECTOR b, float t)は
	// answer = a + ((b-a) * t)
	VECTOR aimPos = VGet(0, player.GetPos().y + 20.0f, player.GetPos().z - 30.0f);
	VECTOR posToAim = VSub(aimPos, pos);
	VECTOR scaledPosToAim = VScale(posToAim, 0.1f);
	pos = VAdd(pos, scaledPosToAim);
#endif
	// カメラに位置を反映.
	SetCameraPositionAndTarget_UpVecY(pos, player.GetPos());
}