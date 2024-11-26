//-----------------------------------------------------------------------------
// @brief  2Dアクションサンプルプログラム.
// 2024 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------

#include "DxLib.h"
#include "Screen.h"
#include "Map.h"
#include "Player.h"
#include "Camera.h"

/// <summary>
/// Main
/// </summary>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetGraphMode(ScreenWidth, ScreenHeight, 32);		// 解像度をSCREEN_W*SCREEN_H、colorを32bitに設定.

	ChangeWindowMode(TRUE);

	// ＤＸライブラリの初期化
	if (DxLib_Init() == -1) return -1;

	// 描画先を裏画面にセット
	SetDrawScreen(DX_SCREEN_BACK);

	// カメラ初期化
	Camera camera;
	InitCamera(camera);

	// プレイヤー初期化
	Player player;
	InitPlayer(player);

	// マップ初期化
	Map map;
	InitMap(map);

	// メインループ開始、ＥＳＣキーで外に出る
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		auto prevTime = GetNowHiPerformanceCount();	// 処理が始まる前の時間
		
		ClearDrawScreen();// 画面のクリア

		// 更新
		UpdatePlayer(player, map);
		UpdateCamera(camera, player);

		// 描画
		DrawPlayer(player, camera);
		DrawMap(map, camera);
		
		ScreenFlip();	// 画面の更新

		// 雑なfps固定処理
		// 差を求めて、1回の画面更新が1/60秒になるようにwhileループ回して待つ
		auto afterTime = GetNowHiPerformanceCount(); // 処理が終わった後の時間
		while (afterTime - prevTime < 16667)
		{
			afterTime = GetNowHiPerformanceCount();
		}
	}

	// ＤＸライブラリの後始末
	DxLib_End();

	// 終了
	return 0;
}