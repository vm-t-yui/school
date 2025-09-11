//-----------------------------------------------------------------------------
// @brief  メイン処理.
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#include "DxLib.h"

// 定数群
const int ScreenW = 640;
const int ScreenH = 480;
const float PlayerSpeed = 3.0f;
const float EnemySpeed = 3.0f;
const VECTOR PlayerFirstPos = VGet(ScreenW * 0.5f, ScreenH - 80.0f, 0);
const VECTOR EnemyFirstPos = VGet(0, 50, 0);

/// <summary>
/// メイン関数
/// </summary>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	// 画面モードの設定
	SetGraphMode(ScreenW, ScreenH, 16);		// 解像度を640*480、colorを16bitに設定.
	ChangeWindowMode(TRUE);			// ウインドウモードに.

	// ＤＸライブラリ初期化処理
	if (DxLib_Init() == -1)
	{
		return -1;
	}

	// グラフィックの描画先を裏画面にセット
	SetDrawScreen(DX_SCREEN_BACK);

	// プレイヤーのグラフィックをメモリにロード＆表示座標を初期化
	VECTOR playerPos = PlayerFirstPos;
	int playerGraph = LoadGraph("data/texture/player.png");
	
	// キャラの画像の大きさを取得。毎度キャストするのがいやなので半分サイズも準備
	int playerW, playerH;
	GetGraphSize(playerGraph,&playerW, &playerH);
	const float playerHalfW = playerW * 0.5f;
	const float playerHalfH = playerH * 0.5f;

	// エネミーのグラフィックをメモリにロード＆表示座標を初期化
	VECTOR enemyPos = EnemyFirstPos;
	int enemyGraph = LoadGraph("data/texture/enemy.png");
	int enemyW, enemyH;
	GetGraphSize(enemyGraph, &enemyW, &enemyH);
	const float enemyHalfW = enemyW * 0.5f;
	const float enemyHalfH = enemyH * 0.5f;

	// エネミーが右移動しているかどうかのフラグをリセット
	bool enemyRightMove = true;

	// ゲームループ.
	while (1)
	{
		// 画面を初期化(真っ黒にする)
		ClearDrawScreen();

		//------------------------------//
		// プレイヤーの操作ルーチン
		//------------------------------//
		{
			// 矢印キーを押していたらプレイヤーを移動させる
			if (CheckHitKey(KEY_INPUT_UP) == 1)
			{
				playerPos.y -= PlayerSpeed;
			}
			if (CheckHitKey(KEY_INPUT_DOWN) == 1)
			{
				playerPos.y += PlayerSpeed;
			}
			if (CheckHitKey(KEY_INPUT_LEFT) == 1)
			{
				playerPos.x -= PlayerSpeed;
			}
			if (CheckHitKey(KEY_INPUT_RIGHT) == 1)
			{
				playerPos.x += PlayerSpeed;
			}

			// プレイヤーが画面左端からはみ出そうになっていたら画面内の座標に戻してあげる
			if (playerPos.x < playerHalfW)
			{
				playerPos.x = playerHalfW;
			}
			if (playerPos.x > ScreenW - playerHalfW)
			{
				playerPos.x = ScreenW - playerHalfW;
			}
			if (playerPos.y < playerHalfH)
			{
				playerPos.y = playerHalfH;
			}
			if (playerPos.y > ScreenH - playerHalfH)
			{
				playerPos.y = ScreenH - playerHalfH;
			}

			// プレイヤーを描画
			DrawRotaGraph3(static_cast<int>(playerPos.x),
				static_cast<int>(playerPos.y),
				static_cast<int>(playerHalfW), static_cast<int>(playerHalfH),
				1.0f,1.0f,
				0.0f,playerGraph,
				FALSE, FALSE);
		}


		//------------------------------//
		// エネミーの移動ルーチン
		//------------------------------//
		{
			// エネミーの座標を移動している方向に移動する
			if (enemyRightMove == true)
			{
				enemyPos.x += EnemySpeed;
			}
			else
			{
				enemyPos.x -= EnemySpeed;
			}

			// エネミーが画面端からでそうになっていたら画面内の座標に戻してあげ、移動する方向も反転する
			if (enemyPos.x > ScreenW - enemyHalfW)
			{
				enemyPos.x = ScreenW - enemyHalfW;
				enemyRightMove = false;
			}
			else if (enemyPos.x < enemyHalfW)
			{
				enemyPos.x = enemyHalfW;
				enemyRightMove = true;
			}

			// エネミーを描画
			DrawRotaGraph3(static_cast<int>(enemyPos.x),
				static_cast<int>(enemyPos.y),
				static_cast<int>(enemyHalfW), static_cast<int>(enemyHalfH),
				1.0f, 1.0f,
				0.0f, enemyGraph,
				FALSE, FALSE);
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