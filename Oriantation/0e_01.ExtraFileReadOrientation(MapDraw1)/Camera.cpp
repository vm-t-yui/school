﻿// 2016 Takeru Yui All Rights Reserved.
#include "Player.h"
#include "Camera.h"
#include "Map.h"

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
	// ほんとは…
	// 大きさ0.5のマップチップをStageDataColNum個配置する
	// プレイヤーの地面のY位置を0としたいので、地上部のマップチップの数は二つ減らした位置で調整
	// その真ん中に表示するので半分を計算で出す
	// プレイヤーのX座標には追従したいのでplayerのXを使う
	// 注視する視点は、カメラと平行にまっすぐz=0地点


	VECTOR playerPos = player.GetPos();
	VECTOR cameraPos = VGet(playerPos.x, playerPos.y + 3.0f, playerPos.z - 10.0f);
	VECTOR lookPos = playerPos;

	pos = cameraPos;

	// カメラに位置を反映.
	SetCameraPositionAndTarget_UpVecY(pos, lookPos);
}