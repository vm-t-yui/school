#include "DxLib.h"
#include "Camera.h"
#include "Player.h"
#include "Screen.h"

// カメラ範囲
const float CameraScopeRangeW = 400.0f;
const float CameraScopeRangeH = 300.0f;

void InitCamera(Camera& camera)
{
	camera.pos = VGet(0, 0, 0);
}

void UpdateCamera(Camera& camera, const Player& player)
{
	// プレイヤーの位置が、カメラの中央から一定以上離れたら
	// カメラのポジションをその範囲内に留める
	if (player.pos.x > camera.pos.x + (CameraScopeRangeW * 0.5f))
	{
		camera.pos.x = player.pos.x - (CameraScopeRangeW * 0.5f);
	}
	else if (player.pos.x < camera.pos.x - (CameraScopeRangeW * 0.5f))
	{
		camera.pos.x = player.pos.x + (CameraScopeRangeW * 0.5f);
	}
	if (player.pos.y > camera.pos.y + (CameraScopeRangeH * 0.5f))
	{
		camera.pos.y = player.pos.y - (CameraScopeRangeH * 0.5f);
	}
	else if (player.pos.y < camera.pos.y - (CameraScopeRangeH * 0.5f))
	{
		camera.pos.y = player.pos.y + (CameraScopeRangeH * 0.5f);
	}

	printfDx("camera.pos:(%d,%d) player.pos:(%d,%d)\n",
		(int)camera.pos.x,
		(int)camera.pos.y,
		(int)player.pos.x,
		(int)player.pos.y);

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