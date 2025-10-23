//-----------------------------------------------------------------------------
// @brief  メイン処理.
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#include "DxLib.h"

// 定数群
constexpr int	ScreenW			= 640;
constexpr int	ScreenH			= 480;
constexpr float	PlayerSpeed		= 3.0f;
constexpr float	EnemySpeed		= 3.0f;
const VECTOR	PlayerFirstPos	= VGet(ScreenW * 0.5f, ScreenH - 80.0f, 0);
const VECTOR	EnemyFirstPos	= VGet(0, 50, 0);
constexpr int	ColorBit		= 16;

/// <summary>
/// メイン関数
/// </summary>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	// 画面モードの設定
	SetGraphMode(ScreenW, ScreenH, ColorBit);		// 解像度を640*480、colorを16bitに設定
	ChangeWindowMode(TRUE);							// ウインドウモードに

	// ＤＸライブラリ初期化処理
	if (DxLib_Init() == -1)
	{
		return -1;
	}

	// グラフィックの描画先を裏画面にセット
	SetDrawScreen(DX_SCREEN_BACK);

	// プレイヤーのグラフィックをメモリにロード＆表示座標を初期化
	VECTOR	playerPos	= PlayerFirstPos;
	int		playerGraph	= LoadGraph("data/texture/player.png");
	
	// キャラの画像の大きさを取得。毎度キャストするのがいやなので半分サイズも準備
	int playerW, playerH;
	GetGraphSize(playerGraph,&playerW, &playerH);

	const float playerHalfW = playerW * 0.5f;
	const float playerHalfH = playerH * 0.5f;

	// エネミーのグラフィックをメモリにロード＆表示座標を初期化
	VECTOR	enemyPos	= EnemyFirstPos;
	VECTOR	enemyDir	= VGet(0, 0, 0);	// エネミーの向き
	int		enemyGraph	= LoadGraph("data/texture/enemy.png");

	int enemyW, enemyH;
	GetGraphSize(enemyGraph, &enemyW, &enemyH);

	const float enemyHalfW = enemyW * 0.5f;
	const float enemyHalfH = enemyH * 0.5f;

	// エネミーが右移動しているかどうかのフラグをリセット
	bool isEnemyRightMove = true;

	// ゲームループ
	while (true)
	{
		// 画面を初期化(真っ黒にする)
		ClearDrawScreen();

		//------------------------------//
		// プレイヤーの操作ルーチン
		//------------------------------//
		{
			VECTOR playerVelocity = VGet(0, 0, 0);	// Velocity

			// 矢印キーを押していたらプレイヤーを移動させる
			if (CheckHitKey(KEY_INPUT_UP) == 1)
			{
				playerVelocity = VGet(0, -1, 0);
			}
			if (CheckHitKey(KEY_INPUT_DOWN) == 1)
			{
				playerVelocity = VGet(0, 1, 0);
			}
			if (CheckHitKey(KEY_INPUT_LEFT) == 1)
			{
				playerVelocity = VGet(-1, 0, 0);
			}
			if (CheckHitKey(KEY_INPUT_RIGHT) == 1)
			{
				playerVelocity = VGet(1, 0, 0);
			}

			// プレイヤーの移動
			// nextPos = pos + velocity
			playerPos = VAdd(playerPos, playerVelocity);			// 座標ベクトルに、velicityを足すことで移動

			// プレイヤーが画面左端からはみ出そうになっていたら画面内の座標に戻してあげる
			if (playerPos.x < 0)
			{
				playerPos.x = 0;
			}
			if (playerPos.x > ScreenW - static_cast<float>(playerW))
			{
				playerPos.x = ScreenW - static_cast<float>(playerW);
			}
			if (playerPos.y < static_cast<float>(playerH))
			{
				playerPos.y = static_cast<float>(playerH);
			}
			if (playerPos.y > ScreenH - static_cast<float>(playerH))
			{
				playerPos.y = ScreenH - static_cast<float>(playerH);
			}

			// プレイヤーを描画
			DrawGraph(static_cast<int>(playerPos.x),
				static_cast<int>(playerPos.y),
				playerGraph, FALSE);
		}

		//------------------------------//
		// エネミーの移動ルーチン
		//------------------------------//
		{
			// エネミーの座標を移動している方向に移動する
			VECTOR enemyVelocity = VGet(0, 0, 0);
			if (isEnemyRightMove == true)
			{
				enemyVelocity = VGet(1, 0, 0);
			}
			else
			{
				enemyVelocity = VGet(-1, 0, 0);
			}

			// エネミーの移動
			enemyPos = VAdd(enemyPos, enemyVelocity);	// 座標ベクトルに、velocityを足すことで移動

			// エネミーが画面端からでそうになっていたら画面内の座標に戻してあげ、移動する方向も反転する
			if (enemyPos.x > ScreenW - static_cast<float>(enemyW))
			{
				enemyPos.x = ScreenW - static_cast<float>(enemyW);
				isEnemyRightMove = false;
			}
			else if (enemyPos.x < 0)
			{
				enemyPos.x = 0;
				isEnemyRightMove = true;
			}

			// エネミーを描画
			DrawGraph(static_cast<int>(enemyPos.x),
				static_cast<int>(enemyPos.y),
				enemyGraph,
				FALSE);
		}

		// 裏画面の内容を表画面にコピーする（描画の確定）
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