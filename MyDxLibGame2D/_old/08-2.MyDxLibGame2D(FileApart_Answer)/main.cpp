//-----------------------------------------------------------------------------
// @brief  メイン処理.
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#include "DxLib.h"
#include "Enemy.h"
#include "Shot.h"
#include "Player.h"

//-----------------------------------------------------------------------------
// @brief  メイン関数.
//-----------------------------------------------------------------------------

// WinMain関数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	// 画面モードの設定
	SetGraphMode(640, 480, 16);		// 解像度を640*480、colorを16bitに設定.
	ChangeWindowMode(TRUE);			// ウインドウモードに.

	// ＤＸライブラリ初期化処理
	if (DxLib_Init() == -1)
	{
		return -1;
	}

	// グラフィックの描画先を裏画面にセット
	SetDrawScreen(DX_SCREEN_BACK);

	Player player;
	InitializePlayer(player);	// プレイヤー初期化

	// エネミーのグラフィックをメモリにロード＆表示座標を初期化
	Enemy enemy;
	InitializeEnemy(enemy);

	Shot shot[SHOT];
	int shotGraph = LoadGraph("data/texture/shot.png");
	for (int i = 0; i < SHOT; i++)
	{
		InitializeShot(shot[i], shotGraph);
	}

	// ゲームループ.
	while (1)
	{
		// 画面を初期化(真っ黒にする)
		ClearDrawScreen();

		//------------------------------//
		// プレイヤールーチン
		//------------------------------//
		UpdatePlayer(player, shot, SHOT);	// プレイヤーを更新する
		DrawPlayer(player);	// プレイヤーを描画する

		//------------------------------//
		// エネミールーチン
		//------------------------------//
		UpdateEnemy(enemy);
		DrawEnemy(enemy);

		//------------------------------//
		// 弾ルーチン
		//------------------------------//
		for (int i = 0; i < SHOT; i++)
		{
			UpdateShot(shot[i], enemy);
			DrawShot(shot[i]);
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

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;					// ソフトの終了
}
