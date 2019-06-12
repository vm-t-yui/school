//-----------------------------------------------------------------------------
// @brief  メイン処理.
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#include "DxLib.h"

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

	// プレイヤーのグラフィックをメモリにロード＆表示座標を初期化
	int playerX, playerY, playerGraph;
	playerGraph = LoadGraph("data/texture/player.png");
	playerX = 288; 
	playerY = 400;

	// エネミーのグラフィックをメモリにロード＆表示座標を初期化
	int enemyX, enemyY, enemyGraph;
	enemyGraph = LoadGraph("data/texture/enemy.png");
	enemyX = 0; 
	enemyY = 50;

	// ショットのグラフィックをメモリにロード.
	int shotGraph;
	shotGraph = LoadGraph("data/texture/shot.png");

	// 位置を初期化.
	int shotX, shotY;

	// 弾が画面上に存在しているか保持する変数に『存在していない』を意味するfalseを代入しておく
	bool shotFlag = false;

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
				playerY -= 3;
			}
			if (CheckHitKey(KEY_INPUT_DOWN) == 1)
			{
				playerY += 3;
			}
			if (CheckHitKey(KEY_INPUT_LEFT) == 1)
			{
				playerX -= 3;
			}
			if (CheckHitKey(KEY_INPUT_RIGHT) == 1)
			{
				playerX += 3;
			}

			// 弾の発射処理
			if (CheckHitKey(KEY_INPUT_SPACE))
			{
				// 弾iが画面上にでていない場合はその弾を画面に出す
				if (shotFlag == false)
				{
					int Bw, Bh, Sw, Sh;

					// プレイヤーと弾の画像のサイズを得る
					GetGraphSize(playerGraph, &Bw, &Bh);
					GetGraphSize(shotGraph, &Sw, &Sh);

					// 弾iの位置をセット、位置はプレイヤーの中心にする
					shotX = (Bw - Sw) / 2 + playerX;
					shotY = (Bh - Sh) / 2 + playerY;

					// 弾iは現時点を持って存在するので、存在状態を保持する変数に１を代入する
					shotFlag = true;
				}
			}

			// プレイヤーが画面左端からはみ出そうになっていたら画面内の座標に戻してあげる
			if (playerX < 0)
			{
				playerX = 0;
			}
			if (playerX > 640 - 64)
			{
				playerX = 640 - 64;
			}
			if (playerY < 0)
			{
				playerY = 0;
			}
			if (playerY > 480 - 64)
			{
				playerY = 480 - 64;
			}

			// プレイヤーを描画
			DrawGraph(playerX, playerY, playerGraph, FALSE);
		}

		//------------------------------//
		// エネミーの移動ルーチン
		//------------------------------//
		{
			// エネミーの座標を移動している方向に移動する
			if (enemyRightMove == true)
			{
				enemyX += 3;
			}
			else
			{
				enemyX -= 3;
			}

			// エネミーが画面端からでそうになっていたら画面内の座標に戻してあげ、移動する方向も反転する
			if (enemyX > 576)
			{
				enemyX = 576;
				enemyRightMove = false;
			}
			else if (enemyX < 0)
			{
				enemyX = 0;
				enemyRightMove = true;
			}

			// エネミーを描画
			DrawGraph(enemyX, enemyY, enemyGraph, FALSE);
		}

		//------------------------------//
		// 弾の移動ルーチン
		//------------------------------//
		// 自機の弾の移動ルーチン( 存在状態を保持している変数の内容がtrue(存在する)の場合のみ行う )
		if (shotFlag == true)
		{
			// 弾iを１６ドット上に移動させる
			shotY -= 16;

			// 画面外に出てしまった場合は存在状態を保持している変数にfalse(存在しない)を代入する
			if (shotY < -80)
			{
				shotFlag = false;
			}

			// 画面に弾iを描画する
			DrawGraph(shotX, shotY, shotGraph, FALSE);
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