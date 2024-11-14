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
	// プレイヤーの位置が、カメラの中央から一定以上離れたら
	// カメラのポジションをその範囲内に留める
	//if (camera.pos.x > ? ? ? )
	//{
	//	? ? ?
	//}
	//else if (camera.pos.x < ? ? ? )
	//{
	//	???
	//}
	//if (camera.pos.y > ? ? ? )
	//{
	//	? ? ?
	//}
	//else if (camera.pos.y < ? ? ? )
	//{
	//	? ? ?
	//}

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