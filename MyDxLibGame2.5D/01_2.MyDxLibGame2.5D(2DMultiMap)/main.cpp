// 2023 Takeru Yui All Rights Reserved.
#include<vector>
#include "DxLib.h"
#include "Player.h"
#include "Map.h"
#include "Camera.h"

/// <summary>
/// メイン関数
/// </summary>
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) 
{
	// ＤＸライブラリ初期化処理
	if (DxLib_Init() == -1)		
	{
		return -1;	// エラーが起きたら直ちに終了
	}

	// 画面モードのセット
	ChangeWindowMode(TRUE);
	SetGraphMode(1600, 900, 16);

	SetDrawScreen(DX_SCREEN_BACK);	// 裏画面を描画対象にする
	SetUseZBufferFlag(TRUE);		// Ｚバッファを使用する
	SetWriteZBufferFlag(TRUE);		// Ｚバッファへの書き込みを行う
	SetUseBackCulling(TRUE);		// バックカリングを行う

	// 生成
	Camera* camera = new Camera();
	Player* player = new Player();
	Map*	map	   = new Map();

	// ロード
	map->Load();

	// エスケープキーが押されるかウインドウが閉じられるまでループ
	LONGLONG frameTime = 0;
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		frameTime = GetNowHiPerformanceCount();
		// プレイヤー制御.
		player->Update();

		// TODO: 定数なのでいい感じの位置に移動したい
		// マップチップのサイズ
		map->Update();
		camera->Update(*player);

		// 画面を初期化する
		ClearDrawScreen();

		// 描画
		map->Draw();
		player->Draw();

		// デバッグ描画
		// XYZ軸
		float lineSize = 300.0f;
		DrawLine3D(VGet(-lineSize, 0, 0), VGet(lineSize, 0, 0), GetColor(255, 0, 0));
		DrawLine3D(VGet(0, -lineSize, 0), VGet(0, lineSize, 0), GetColor(0, 255, 0));
		DrawLine3D(VGet(0, 0, -lineSize), VGet(0, 0, lineSize), GetColor(0, 0, 255));

		// マップチップライン(StageDataColNum個のマップチップをどう配置するか)
		// プレイヤーの地面をY=0としたいので、その周りを配置し、大体の基準でカメラを決める
		for (int i = 0; i < Map::StageDataColNum + 2; i++)
		{
			// X軸とかぶるところはとりあえず描画しない
			if (i != 1)
			{
				float y = Map::ChipSize * (i - 1); // 一個下のラインからチップが始まる
				DrawLine3D(VGet(-lineSize, y, 0), VGet(lineSize, y, 0), GetColor(255, 255, 0));
			}
		}

		// 裏画面の内容を表画面に反映させる
		ScreenFlip();

		// 雑に60fps固定
		while ((GetNowHiPerformanceCount() - frameTime) < 16667) {}
	}

	// 後始末
	delete(player);
	delete(camera);
	delete(map);

	// ＤＸライブラリの後始末
	DxLib_End();

	// ソフトの終了
	return 0;
}