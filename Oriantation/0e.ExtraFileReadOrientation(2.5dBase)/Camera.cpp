// 2016 Takeru Yui All Rights Reserved.
#include "Player.h"
#include "Camera.h"

/// <summary>
/// コンストラクタ
/// </summary>
Camera::Camera()
{
	//奥行0.1～1000までをカメラの描画範囲とする
	SetCameraNearFar(0.1f, 1000.0f);

	pos = VGet(0, 0, 0);
}

/// <summary>
/// デストラクタ
/// </summary>
Camera::~Camera()
{
	// 処理なし.
}

/// <summary>
/// 更新
/// </summary>
void Camera::Update(const Player& player)
{
	VECTOR pos = VAdd(player.GetPos(), VGet(0, 0, -10.0f));
	VECTOR lookPos = VAdd(player.GetPos(), VGet(0, 1.0f, 0));

	// カメラに位置を反映.
	SetCameraPositionAndTarget_UpVecY(pos, lookPos);
}