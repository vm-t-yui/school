#include "DxLib.h"
#include "Camera.h"
#include "Player.h"
#include "Screen.h"

void InitCamera(Camera& camera)
{
	camera.pos = VGet(0, 0, 0);
}

void UpdateCamera(Camera& camera, const Player& player)
{
	//// カメラを固定方向に動かす ////
	// カメラが右に移動（表示物は全て左に移動）
	//camera.pos.x += 0.1f;	// そのままだと反対に表示物が動く

	// カメラが下に移動（表示物は全て上に移動）
	//camera.pos.y += 0.1f;	// そのままだと反対に表示物が動く

	//// プレイヤーの位置とカメラの位置を同じにする ////
	camera.pos = player.pos;

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