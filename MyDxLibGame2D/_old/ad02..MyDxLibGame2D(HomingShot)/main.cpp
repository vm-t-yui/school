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
	VECTOR playerPos;
	int playerGraph;
	playerGraph = LoadGraph("data/texture/player.png");
	playerPos.x = 288; 
	playerPos.y = 400;

	// エネミーのグラフィックをメモリにロード＆表示座標を初期化
	VECTOR enemyPos;
	int enemyGraph;
	enemyGraph = LoadGraph("data/texture/enemy.png");
	enemyPos.x = 0; 
	enemyPos.y = 50;

	// エネミーが右移動しているかどうかのフラグをリセット
	bool enemyRightMove = true;

	// ショットのグラフィックをメモリにロード.
	int shotGraph;
	shotGraph = LoadGraph("data/texture/shot.png");

	// ショットの生きている時間カウンタ
	int shotLifeCounter = 0;
	VECTOR shotDir = VGet(0,0,0);

	// 位置を初期化.
	VECTOR shotPos;

	// 弾が画面上に存在しているか保持する変数に『存在していない』を意味するfalseを代入しておく
	bool shotFlag = false;

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
			VECTOR direction = VGet(0, 0, 0);	// direction = (0,0,0)
			float playerSpeed = 5.0f;
;			if (CheckHitKey(KEY_INPUT_UP) == 1)
			{
				direction = VAdd(direction, VGet(0, -1, 0)); // direction += (0,-1,0) 
			}
			if (CheckHitKey(KEY_INPUT_DOWN) == 1)
			{
				direction = VAdd(direction, VGet(0, 1, 0)); // direction += (0,1,0) 
			}
			if (CheckHitKey(KEY_INPUT_LEFT) == 1)
			{
				direction = VAdd(direction, VGet(-1, 0, 0)); // direction += (-1,0,0) 
			}
			if (CheckHitKey(KEY_INPUT_RIGHT) == 1)
			{
				direction = VAdd(direction, VGet(1, 0, 0)); // direction += (1,0,0) 
			}
			if (VSize(direction) > 0)						// 長さがゼロより大きい時だけ
			{
				direction = VNorm(direction);				// directionの長さを１に正規化
			}

			// 長さが１になったdirectionにspeedをかける
			VECTOR velocity = VScale(direction, playerSpeed);	// velocity = direction * playerSpeed

			playerPos = VAdd(playerPos, velocity); // playerPos = playerPos + velociity

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
					shotPos.x = (Bw - Sw) / 2 + playerPos.x;
					shotPos.y = (Bh - Sh) / 2 + playerPos.y;

					// 弾iは現時点を持って存在するので、存在状態を保持する変数に１を代入する
					shotFlag = true;
					shotLifeCounter = 0;
				}
			}

			// プレイヤーが画面左端からはみ出そうになっていたら画面内の座標に戻してあげる
			if (playerPos.x < 0)
			{
				playerPos.x = 0;
			}
			if (playerPos.x > 640 - 64)
			{
				playerPos.x = 640 - 64;
			}
			if (playerPos.y < 0)
			{
				playerPos.y = 0;
			}
			if (playerPos.y > 480 - 64)
			{
				playerPos.y = 480 - 64;
			}

			// プレイヤーを描画
			DrawGraph(playerPos.x, playerPos.y, playerGraph, FALSE);
		}


		//------------------------------//
		// エネミーの移動ルーチン
		//------------------------------//
		{
			// エネミーの座標を移動している方向に移動する
			VECTOR enemyDirection = VGet(0,0,0);
			float enemySpeed = 3.0f;
			if (enemyRightMove == true)
			{
				enemyDirection = VAdd(enemyDirection, VGet(1, 0, 0));
			}
			else
			{
				enemyDirection = VAdd(enemyDirection, VGet(-1, 0, 0));
			}
			if (VSize(enemyDirection) > 0)
			{
				enemyDirection = VNorm(enemyDirection);
			}
			VECTOR enemyVelocity = VScale(enemyDirection, enemySpeed);
			enemyPos = VAdd(enemyPos, enemyVelocity);

			// エネミーが画面端からでそうになっていたら画面内の座標に戻してあげ、移動する方向も反転する
			if (enemyPos.x > 576)
			{
				enemyPos.x = 576;
				enemyRightMove = false;
			}
			else if (enemyPos.x < 0)
			{
				enemyPos.x = 0;
				enemyRightMove = true;
			}

			// エネミーを描画
			DrawGraph(enemyPos.x, enemyPos.y, enemyGraph, FALSE);
		}

		//------------------------------//
		// 弾の移動ルーチン
		//------------------------------//
		// 自機の弾の移動ルーチン( 存在状態を保持している変数の内容がtrue(存在する)の場合のみ行う )
		if (shotFlag == true)
		{
			// フレームごとにカウントが増える
			++shotLifeCounter;

			// 弾iを１６ドット上に移動させる
			float shotSpeed = 16.0f;
			
			// うたれてから15フレームの間はshotDirを敵の方向にする
			if (shotLifeCounter <= 15)
			{
				shotDir = VSub(enemyPos, shotPos);	// enemyPos - shotPos
			}

			if (VSize(shotDir) > 0)
			{
				shotDir = VNorm(shotDir);
			}
			VECTOR shotVelocity = VScale(shotDir, shotSpeed);
			shotPos = VAdd(shotPos, shotVelocity);

			// 画面外に出てしまった場合は存在状態を保持している変数にfalse(存在しない)を代入する
			if (shotPos.y < -80)
			{
				shotFlag = false;
			}

			// 画面に弾iを描画する
			DrawGraph(shotPos.x, shotPos.y, shotGraph, FALSE);
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