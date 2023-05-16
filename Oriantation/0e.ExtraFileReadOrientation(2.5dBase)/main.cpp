// 2023 Takeru Yui All Rights Reserved.
#include "DxLib.h"
#include "Player.h"
#include "Camera.h"

/// <summary>
/// メイン関数
/// </summary>
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) 
{
	ChangeWindowMode(TRUE);

	// ＤＸライブラリ初期化処理
	if (DxLib_Init() == -1)		
	{
		return -1;	// エラーが起きたら直ちに終了
	}

	// 画面モードのセット
	SetGraphMode(1600, 900, 16);

	// カメラを生成.
	Camera* camera = new Camera();

	// プレイヤーを生成.
	Player* player = new Player();

	// エスケープキーが押されるかウインドウが閉じられるまでループ
	LONGLONG frameTime = 0;
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		frameTime = GetNowHiPerformanceCount();
		// プレイヤー制御.
		player->Update();
		camera->Update(*player);

		// 画面を初期化する
		ClearDrawScreen();

		// 描画
		player->Draw();

		// デバッグ描画
		float lineSize = 10.0f;
		DrawLine3D(VGet(-lineSize, 0, 0), VGet(lineSize, 0, 0), GetColor(255, 0, 0));
		DrawLine3D(VGet(0, -lineSize, 0), VGet(0, lineSize, 0), GetColor(0, 255, 0));
		DrawLine3D(VGet(0, 0, -lineSize), VGet(0, 0, lineSize), GetColor(0, 0, 255));

		// 裏画面の内容を表画面に反映させる
		ScreenFlip();

		// 雑に60fps固定
		while ((GetNowHiPerformanceCount() - frameTime) < 16667) {}
	}

	// 後始末
	delete(player);
	delete(camera);

	// ＤＸライブラリの後始末
	DxLib_End();

	// ソフトの終了
	return 0;
}