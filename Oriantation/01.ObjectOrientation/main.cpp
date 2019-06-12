//-----------------------------------------------------------------------------
// @brief  メイン処理.
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#include "DxLib.h"

//-----------------------------------------------------------------------------
// @brief  メイン関数.
//-----------------------------------------------------------------------------
// ショットの数.
#define SHOT 20

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

	// エネミーのグラフィックをメモリにロード＆ダメージ時の状態管理用の変数を初期化
	int enemyDamageFlag, enemyDamageCounter, enemyDamageGraph;
	enemyDamageGraph = LoadGraph("data/texture/enemyDamage.png");

	// エネミーが顔を歪めているかどうかの変数に『歪めていない』を表す０を代入
	enemyDamageFlag = 0;

	// ショットのグラフィックをメモリにロード.
	int shotGraph;
	shotGraph = LoadGraph("data/texture/shot.png");

	// 弾が画面上に存在しているか保持する変数に『存在していない』を意味するfalseを代入しておく
	int shotX[SHOT], shotY[SHOT];
	bool shotFlag[SHOT];
	for (int i = 0; i < SHOT; i++)
	{
		shotFlag[i] = false;
	}

	// 弾のグラフィックのサイズをえる
	int shotW, shotH;
	GetGraphSize(shotGraph, &shotW, &shotH);

	// エネミーのグラフィックのサイズを得る
	int enemyW, enemyH;
	GetGraphSize(enemyGraph, &enemyW, &enemyH);

	// ショットボタンが前のフレームで押されたかどうかを保存する変数にfalse(押されいない)を代入
	bool prevshotFlag = false;

	// エネミーが右移動しているかどうかのフラグをリセット
	bool enemyRightMove = true;

	// ゲームループ.
	while (1)
	{
		// 画面を初期化(真っ黒にする)
		ClearDrawScreen();

		//------------------------------//
		// プレイヤールーチン
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
				// 前フレームでショットボタンを押したかが保存されている変数がfalseだったら弾を発射
				if (prevshotFlag == false)
				{
					// 画面上にでていない弾があるか、弾の数だけ繰り返して調べる
					for (int i = 0; i < SHOT; i++)
					{
						// 弾iが画面上にでていない場合はその弾を画面に出す
						if (shotFlag[i] == false)
						{
							int Bw, Bh, Sw, Sh;

							// プレイヤーと弾の画像のサイズを得る
							GetGraphSize(playerGraph, &Bw, &Bh);
							GetGraphSize(shotGraph, &Sw, &Sh);

							// 弾iの位置をセット、位置はプレイヤーの中心にする
							shotX[i] = (Bw - Sw) / 2 + playerX;
							shotY[i] = (Bh - Sh) / 2 + playerY;

							// 弾iは現時点を持って存在するので、存在状態を保持する変数にtrueを代入する
							shotFlag[i] = true;

							// 一つ弾を出したので弾を出すループから抜けます
							break;
						}
					}
				}

				// 前フレームでショットボタンを押されていたかを保存する変数にtrue(おされていた)を代入
				prevshotFlag = true;
			}
			else
			{
				// ショットボタンが押されていなかった場合は
				// 前フレームでショットボタンが押されていたかを保存する変数にfalse(おされていない)を代入
				prevshotFlag = false;
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
		// エネミールーチン
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
			// 顔を歪めているかどうかで処理を分岐
			if (enemyDamageFlag == 1)
			{
				// 顔を歪めている場合はダメージ時のグラフィックを描画する
				DrawGraph(enemyX, enemyY, enemyDamageGraph, FALSE);

				// 顔を歪めている時間を測るカウンターに１を加算する
				enemyDamageCounter++;

				// もし顔を歪め初めて ３０ フレーム経過していたら顔の歪んだ状態から
				// 元に戻してあげる
				if (enemyDamageCounter == 30)
				{
					// 『歪んでいない』を表す０を代入
					enemyDamageFlag = 0;
				}
			}
			else
			{
				DrawGraph(enemyX, enemyY, enemyGraph, FALSE);
			}
		}

		//------------------------------//
		// 弾ルーチン
		//------------------------------//
		for (int i = 0; i < SHOT; i++)
		{
			// 自機の弾iの移動ルーチン( 存在状態を保持している変数の内容がtrue(存在する)の場合のみ行う )
			if (shotFlag[i] == true)
			{
				// 弾iを１６ドット上に移動させる
				shotY[i] -= 16;

				// 画面外に出てしまった場合は存在状態を保持している変数にfalse(存在しない)を代入する
				if (shotY[i] < -80)
				{
					shotFlag[i] = false;
				}

				// 画面に弾iを描画する
				DrawGraph(shotX[i], shotY[i], shotGraph, FALSE);
			}

			// 弾のあたり判定.
			// 弾iが存在している場合のみ次の処理に映る
			if (shotFlag[i] == 1)
			{
				// エネミーとの当たり判定
				if (((shotX[i] > enemyX && shotX[i] < enemyX + enemyW) ||
					(enemyX > shotX[i] && enemyX < shotX[i] + shotW)) &&
					((shotY[i] > enemyY && shotY[i] < enemyY + enemyH) ||
						(enemyY > shotY[i] && enemyY < shotY[i] + shotH)))
				{
					// 接触している場合は当たった弾の存在を消す
					shotFlag[i] = 0;

					// エネミーの顔を歪めているかどうかを保持する変数に『歪めている』を表す１を代入
					enemyDamageFlag = 1;

					// エネミーの顔を歪めている時間を測るカウンタ変数に０を代入
					enemyDamageCounter = 0;
				}
			}
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