//-----------------------------------------------------------------------------
// @brief  メイン処理.
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#include "DxLib.h"
#include "Player.h"
#include "Camera.h"
#include "ObstructManager.h"
#include "HitChecker.h"

//-----------------------------------------------------------------------------
// @brief  メイン関数.
//-----------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) 
{
	// ＤＸライブラリ初期化処理
	if (DxLib_Init() == -1)		
	{
		return -1;	// エラーが起きたら直ちに終了
	}

	// 画面モードのセット
	SetGraphMode(640, 480, 16);
	ChangeWindowMode(TRUE);

	// カメラを生成.
	Camera* camera = new Camera();

	// プレイヤーを生成.
	Player* player = new Player();

	// 障害物を生成.
	ObstructManager* obstructManager = new ObstructManager();
	obstructManager->CreateObstructs();

	// エスケープキーが押されるかウインドウが閉じられるまでループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// プレイヤー制御.
		player->Update();

		// カメラ制御.
		camera->Update(*player);

		// 障害物制御.
		obstructManager->Update();

		// ヒットのチェック.
		HitChecker::Check(*player, *obstructManager);

		// 画面を初期化する
		ClearDrawScreen();

		// プレイヤー描画.
		player->Draw();

		// 障害物描画.
		obstructManager->Draw();

		// 裏画面の内容を表画面に反映させる
		ScreenFlip();
	}

	// プレイヤーを削除.
	delete(player);

	// カメラを削除.
	delete(camera);

	// 障害物を削除.
	obstructManager->DestroyObstructs();
	delete(obstructManager);

	// ＤＸライブラリの後始末
	DxLib_End();

	// ソフトの終了
	return 0;
}