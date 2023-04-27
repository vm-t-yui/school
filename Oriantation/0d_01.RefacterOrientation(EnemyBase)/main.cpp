// 2023 Takeru Yui All Rights Reserved.
#include "DxLib.h"
#include "Common.h"
#include "Player.h"
#include "Shot.h"
#include "Enemy.h"
#include "Enemy2.h"
#include "Enemy3.h"

/// <summary>
/// WinMain関数
/// </summary>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	// 画面モードの設定
	SetGraphMode(SCREEN_W, SCREEN_H, 16);		// 解像度をSCREEN_W*SCREEN_H、colorを16bitに設定.
	ChangeWindowMode(TRUE);			// ウインドウモードに.
	
	// ＤＸライブラリ初期化処理
	if (DxLib_Init() == -1)
	{
		return -1;
	}

	// 背景のスクロールをするためにテクスチャモードをラッピングに修正.
	SetTextureAddressMode(DX_TEXADDRESS_WRAP);

	// グラフィックの描画先を裏画面にセット
	SetDrawScreen(DX_SCREEN_BACK);

	Player player;
	const int EnemyNum = 3;
	EnemyBase* enemies[EnemyNum];
	enemies[0] = new Enemy();
	enemies[1] = new Enemy2();
	enemies[2] = new Enemy3();

	Shot shot[SHOT_NUM]{};

	// 各初期化を呼ぶ.
	player.Init();
	for (int i = 0; i < EnemyNum; i++)
	{
		enemies[i]->Init();
	}
	for (int i = 0; i < SHOT_NUM; i++)
	{
		shot[i].Init();
	}

	// ゲームループ.
	while (1)
	{
		// 画面を初期化(真っ黒にする)
		ClearDrawScreen();

		// 各アップデート関数を呼ぶ.
		player.Update(shot, SHOT_NUM);
		for (int i = 0; i < 3; i++)
		{
			enemies[i]->Update();
		}
		for (int i = 0; i < SHOT_NUM; i++)
		{
			shot[i].Update(enemies, EnemyNum);
		}

		// 各描画関数を呼ぶ.
		for (int i = 0; i < SHOT_NUM; i++)
		{
			shot[i].Draw();
		}
		player.Draw();
		for (int i = 0; i < EnemyNum; i++)
		{
			enemies[i]->Draw();
		}

		// 裏画面の内容を表画面にコピーする（描画の確定）.
		ScreenFlip();

		// Windows 特有の面倒な処理をＤＸライブラリにやらせる
		// マイナスの値（エラー値）が返ってきたらループを抜ける
		if (ProcessMessage() < 0)
		{
			break;
		}
		// もしＥＳＣキーが押されていたらループから抜ける
		else if (CheckHitKey(KEY_INPUT_ESCAPE))
		{
			break;
		}
	}

	for (int i = 0; i < EnemyNum; i++)
	{
		delete enemies[i];
		enemies[i] = nullptr;
	}

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;					// ソフトの終了
}