#include <cmath>
#include "DxLib.h"
#include "Camera.h"
#include "Player.h"
#include "Screen.h"

// カメラ範囲
const float CameraScopeRangeW = 400.0f;
const float CameraScopeRangeH = 300.0f;
const float CameraScopeRangeTop = 200.0f;
const float CameraScopeRangeBottom = 320.0f;
const float CameraLerpRate = 0.1f;

// Lerp計算をベクトルに行う
VECTOR VLerp(const VECTOR& start, const VECTOR& end, float t)
{
	VECTOR ret;
	ret.x = std::lerp(start.x, end.x, t);
	ret.y = std::lerp(start.y, end.y, t);
	ret.z = std::lerp(start.z, end.z, t);
	return ret;
}

void InitCamera(Camera& camera)
{
	camera.pos = VGet(0, 0, 0);
}

void UpdateCamera(Camera& camera, const Player& player)
{
	// プレイヤーの位置が、カメラの中央から一定以上離れたら
	// カメラの「目標」ポジションをその範囲内に留める
	VECTOR aimCameraPos = camera.pos;
	if (player.pos.x > camera.pos.x + (CameraScopeRangeW * 0.5f))
	{
		aimCameraPos.x = player.pos.x - (CameraScopeRangeW * 0.5f);
	}
	else if (player.pos.x < camera.pos.x - (CameraScopeRangeW * 0.5f))
	{
		aimCameraPos.x = player.pos.x + (CameraScopeRangeW * 0.5f);
	}
	if (player.pos.y > camera.pos.y + (CameraScopeRangeH * 0.5f))
	{
		aimCameraPos.y = player.pos.y - (CameraScopeRangeH * 0.5f);
	}
	else if (player.pos.y < camera.pos.y - (CameraScopeRangeH * 0.5f))
	{
		aimCameraPos.y = player.pos.y + (CameraScopeRangeH * 0.5f);
	}

	// 目標ポジションに、Lerpを使ってカメラポジションを近づける
	camera.pos = VLerp(camera.pos, aimCameraPos, CameraLerpRate);

	// 地面の一定より下にカメラが移動しないように後で固定
	if (camera.pos.y > CameraScopeRangeBottom)
	{
		camera.pos.y = CameraScopeRangeBottom;
	}
	// 空も同じく止める
	if (camera.pos.y < CameraScopeRangeTop)
	{
		camera.pos.y = CameraScopeRangeTop;
	}

	//printfDx("camera.pos:(%d,%d) player.pos:(%d,%d)\n",
	//	(int)camera.pos.x,
	//	(int)camera.pos.y,
	//	(int)player.pos.x,
	//	(int)player.pos.y);

	/////////////////////////////////////////////////////////////////////////////
	// ↓drawOffset補正
	// （カメラを動かしている感覚で他の描画物のポジションをいじれるようにする計算
	/////////////////////////////////////////////////////////////////////////////
	// ↑ベクトルや計算を使って、「カメラのポジションを動かす」
	// という感覚を保ちたいので、Draw側に足しているcamera.posをいじる
	// Draw側に足しているcamera.pos.x/yは反転させる
	camera.drawOffset.x = camera.pos.x * -1;
	camera.drawOffset.y = camera.pos.y * -1;

	// その時、画面の中央にプレイヤーが来るようにする
	// (camera.posが画面の中央になるようにする)
	camera.drawOffset.x = camera.drawOffset.x + (ScreenWidth * 0.5f);
	camera.drawOffset.y = camera.drawOffset.y + (ScreenHeight * 0.5f);
}

#if _DEBUG
// ゲーム上のポジションからスクリーンのポジションに変換する
VECTOR WorldToScreenPos(const Camera& camera, const VECTOR& worldPos)
{
	VECTOR ret = VGet( 
		worldPos.x + camera.drawOffset.x,
		worldPos.y + camera.drawOffset.y,
		0);
	return ret;
}
void DrawCameraDebug(const Camera& camera)
{
	/////////////////////////////////////////////////////////////////////////////
	// カメラ範囲をデバッグで表示
	/////////////////////////////////////////////////////////////////////////////
	auto screenCameraPos = WorldToScreenPos(camera, camera.pos);
	// 上辺
	DrawLine(static_cast<int>(screenCameraPos.x - CameraScopeRangeW * 0.5f),
		static_cast<int>(screenCameraPos.y - CameraScopeRangeH * 0.5f),
		static_cast<int>(screenCameraPos.x + CameraScopeRangeW * 0.5f),
		static_cast<int>(screenCameraPos.y - CameraScopeRangeH * 0.5f),
		0xff00ff);
	// 下辺
	DrawLine(static_cast<int>(screenCameraPos.x - CameraScopeRangeW * 0.5f),
		static_cast<int>(screenCameraPos.y + CameraScopeRangeH * 0.5f),
		static_cast<int>(screenCameraPos.x + CameraScopeRangeW * 0.5f),
		static_cast<int>(screenCameraPos.y + CameraScopeRangeH * 0.5f),
		0xff00ff);
	// 左辺
	DrawLine(static_cast<int>(screenCameraPos.x - CameraScopeRangeW * 0.5f),
		static_cast<int>(screenCameraPos.y - CameraScopeRangeH * 0.5f),
		static_cast<int>(screenCameraPos.x - CameraScopeRangeW * 0.5f),
		static_cast<int>(screenCameraPos.y + CameraScopeRangeH * 0.5f),
		0xff00ff);
	// 右辺
	DrawLine(static_cast<int>(screenCameraPos.x + CameraScopeRangeW * 0.5f),
		static_cast<int>(screenCameraPos.y - CameraScopeRangeH * 0.5f),
		static_cast<int>(screenCameraPos.x + CameraScopeRangeW * 0.5f),
		static_cast<int>(screenCameraPos.y + CameraScopeRangeH * 0.5f),
		0xff00ff);
	// カメラ追わないライン
	// 上
	auto screenScopeTopPos = WorldToScreenPos(camera, VGet(0, CameraScopeRangeTop, 0));
	DrawLine(0,
		static_cast<int>(screenScopeTopPos.y),
		ScreenWidth,
		static_cast<int>(screenScopeTopPos.y),
		0xaa00ff);
	// 下
	auto screenScopeBottomPos = WorldToScreenPos(camera, VGet(0, CameraScopeRangeBottom, 0));
	DrawLine(0,
		static_cast<int>(screenScopeBottomPos.y),
		ScreenWidth,
		static_cast<int>(screenScopeBottomPos.y),
		0xaa00ff);
	// カメラ位置
	DrawBox(static_cast<int>(screenCameraPos.x - 2),
		static_cast<int>(screenCameraPos.y - 2), 
		static_cast<int>(screenCameraPos.x + 2), 
		static_cast<int>(screenCameraPos.y + 2),
		0xaa00ff, false);
}
#endif
