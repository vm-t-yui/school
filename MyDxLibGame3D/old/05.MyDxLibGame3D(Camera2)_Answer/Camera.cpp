﻿//-----------------------------------------------------------------------------
// @brief  カメラクラス.
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#include "Player.h"
#include "Camera.h"

#define _USE_MATH_DEFINES
#include <math.h>

#define USE_LERP_CAMERA 1

//-----------------------------------------------------------------------------
// @brief  コンストラクタ.
//-----------------------------------------------------------------------------
Camera::Camera()
{
	//奥行0.1～1000までをカメラの描画範囲とする
	SetCameraNearFar(0.1f, 1000.0f);

	// FOV(視野角)を60度に
	SetupCamera_Perspective(60.0f * (static_cast<float>(M_PI) / 180.0f));

	pos = VGet(0, 0, 0);
}

//-----------------------------------------------------------------------------
// @brief  デストラクタ.
//-----------------------------------------------------------------------------
Camera::~Camera()
{
	// 処理なし.
}

//-----------------------------------------------------------------------------
// @brief  更新.
//-----------------------------------------------------------------------------
void Camera::Update(const Player& player)
{
#if !USE_LERP_CAMERA
	// z軸上で、プレイヤーから一定距離離れた状態でプレイヤーを常に見続けるよう位置調整
	pos = VGet(0, player.GetPos().y + 20.0f, player.GetPos().z - 30.0f);
#else
	// lerpを使用して実装.
	// lerp(VECTOR a, VECTOR b, float t)は
	// answer = a + ((b-a) * t)
	VECTOR aimPos = VGet(0, player.GetPos().y + 20.0f, player.GetPos().z - 30.0f); // b
	VECTOR posToAim = VSub(aimPos, pos);	// (b-a)
	VECTOR scaledPosToAim = VScale(posToAim, 0.1f);	// ((b-a) * t)
	pos = VAdd(pos, scaledPosToAim);	// a + ((b-a) * t)
#endif
	// カメラに位置を反映.
	SetCameraPositionAndTarget_UpVecY(pos, player.GetPos());
}