﻿//-----------------------------------------------------------------------------
// @brief  メイン処理.
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#include "DxLib.h"

//-----------------------------------------------------------------------------
// @brief  メイン関数.
//-----------------------------------------------------------------------------
// ショットの数.
#define SHOT 20

// プレイヤー構造体.
struct Player
{
	int Graph;
	int X;
	int Y;
	int W;
	int H;
	bool PrevShotFlag;
};

// エネミー構造体.
struct Enemy
{
	int		X;
	int		Y; 
	int		Graph;
	bool	DamageFlag;
	int		DamageCounter;
	int		DamageGraph;
	int		W;
	int		H;
	bool    RightMove;
};

// ショット構造体.
struct Shot
{
	int		Graph;
	int		X;
	int		Y;
	bool	visibleFlag;
	int		W;
	int		H;
};

// プレイヤー初期化
void InitializePlayer(Player& player)
{
	// プレイヤーのグラフィックをメモリにロード＆表示座標を初期化
	player.Graph = LoadGraph("data/texture/player.png");
	player.X = 288;
	player.Y = 400;

	// ショットボタンが前のフレームで押されたかどうかを保存する変数にfalse(押されいない)を代入
	player.PrevShotFlag = false;

	// プレイヤーと弾の画像のサイズを得る
	GetGraphSize(player.Graph, &player.W, &player.H);
}

// プレイヤーの更新処理
void UpdatePlayer(Player& player, Shot shot[], int shotNum)
{
	// 矢印キーを押していたらプレイヤーを移動させる
	if (CheckHitKey(KEY_INPUT_UP) == 1)
	{
		player.Y -= 3;
	}
	if (CheckHitKey(KEY_INPUT_DOWN) == 1)
	{
		player.Y += 3;
	}
	if (CheckHitKey(KEY_INPUT_LEFT) == 1)
	{
		player.X -= 3;
	}
	if (CheckHitKey(KEY_INPUT_RIGHT) == 1)
	{
		player.X += 3;
	}

	// 弾の発射処理
	if (CheckHitKey(KEY_INPUT_SPACE))
	{
		// 前フレームでショットボタンを押したかが保存されている変数がfalseだったら弾を発射
		if (player.PrevShotFlag == false)
		{
			// 画面上にでていない弾があるか、弾の数だけ繰り返して調べる
			for (int i = 0; i < shotNum; i++)
			{
				// 弾iが画面上にでていない場合はその弾を画面に出す
				if (shot[i].visibleFlag == false)
				{
					// 弾iの位置をセット、位置はプレイヤーの中心にする
					shot[i].X = (player.W - shot[i].W) / 2 + player.X;
					shot[i].Y = (player.H - shot[i].H) / 2 + player.Y;

					// 弾iは現時点を持って存在するので、存在状態を保持する変数にtrueを代入する
					shot[i].visibleFlag = true;

					// 一つ弾を出したので弾を出すループから抜けます
					break;
				}
			}
		}

		// 前フレームでショットボタンを押されていたかを保存する変数にtrue(おされていた)を代入
		player.PrevShotFlag = true;
	}
	else
	{
		// ショットボタンが押されていなかった場合は
		// 前フレームでショットボタンが押されていたかを保存する変数にfalse(おされていない)を代入
		player.PrevShotFlag = false;
	}

	// プレイヤーが画面左端からはみ出そうになっていたら画面内の座標に戻してあげる
	if (player.X < 0)
	{
		player.X = 0;
	}
	if (player.X > 640 - player.W)
	{
		player.X = 640 - player.W;
	}
	if (player.Y < 0)
	{
		player.Y = 0;
	}
	if (player.Y > 480 - player.H)
	{
		player.Y = 480 - player.H;
	}
}

// プレイヤーの描画処理
void DrawPlayer(Player& player)
{
	// プレイヤーを描画
	DrawGraph(player.X, player.Y, player.Graph, FALSE);
}

// エネミー初期化
void InitializeEnemy(Enemy& enemy)
{
	enemy.Graph = LoadGraph("data/texture/enemy.png");
	enemy.X = 0;
	enemy.Y = 50;

	// エネミーのグラフィックをメモリにロード＆ダメージ時の状態管理用の変数を初期化
	enemy.DamageGraph = LoadGraph("data/texture/enemyDamage.png");

	// エネミーが顔を歪めているかどうかの変数に『歪めていない』を表すFALSEを代入
	enemy.DamageFlag = false;

	// エネミーのグラフィックのサイズを得る
	GetGraphSize(enemy.Graph, &enemy.W, &enemy.H);

	// エネミーが右移動しているかどうかのフラグをリセット
	enemy.RightMove = true;
}

// エネミー更新
void UpdateEnemy(Enemy& enemy)
{
	// エネミーの座標を移動している方向に移動する
	if (enemy.RightMove == true)
	{
		enemy.X += 3;
	}
	else
	{
		enemy.X -= 3;
	}

	// エネミーが画面端からでそうになっていたら画面内の座標に戻してあげ、移動する方向も反転する
	if (enemy.X > 640 - enemy.W)
	{
		enemy.X = 640 - enemy.W;
		enemy.RightMove = false;
	}
	else if (enemy.X < 0)
	{
		enemy.X = 0;
		enemy.RightMove = true;
	}
}

// エネミー描画
void DrawEnemy(Enemy& enemy)
{
	// エネミーを描画
	// 顔を歪めているかどうかで処理を分岐
	if (enemy.DamageFlag == true)
	{
		// 顔を歪めている場合はダメージ時のグラフィックを描画する
		DrawGraph(enemy.X, enemy.Y, enemy.DamageGraph, FALSE);

		// 顔を歪めている時間を測るカウンターに１を加算する
		enemy.DamageCounter++;

		// もし顔を歪め初めて ３０ フレーム経過していたら顔の歪んだ状態から
		// 元に戻してあげる
		if (enemy.DamageCounter == 30)
		{
			// 『歪んでいない』を表すFALSEを代入
			enemy.DamageFlag = false;
		}
	}
	else
	{
		DrawGraph(enemy.X, enemy.Y, enemy.Graph, FALSE);
	}
}

// ショット初期化
void InitializeShot(Shot& shot)
{
	// ショットのグラフィックをメモリにロード.
	shot.Graph = LoadGraph("data/texture/shot.png");

	// 弾が画面上に存在しているか保持する変数に『存在していない』を意味するfalseを代入しておく
	shot.visibleFlag = false;

	// 弾のグラフィックのサイズをえる
	GetGraphSize(shot.Graph, &shot.W, &shot.H);
}

// ショット更新
void UpdateShot(Shot& shot, Enemy& enemy)
{
	// 弾のあたり判定.
	// 弾iが存在している場合のみ次の処理に映る
	if (shot.visibleFlag == 1)
	{
		// エネミーとの当たり判定
		if (((shot.X > enemy.X && shot.X < enemy.X + enemy.W) ||
			(enemy.X > shot.X && enemy.X < shot.X + shot.W)) &&
			((shot.Y > enemy.Y && shot.Y < enemy.Y + enemy.H) ||
				(enemy.Y > shot.Y && enemy.Y < shot.Y + shot.H)))
		{
			// 接触している場合は当たった弾の存在を消す
			shot.visibleFlag = 0;

			// エネミーの顔を歪めているかどうかを保持する変数に『歪めている』を表すTRUEを代入
			enemy.DamageFlag = true;

			// エネミーの顔を歪めている時間を測るカウンタ変数に０を代入
			enemy.DamageCounter = 0;
		}
	}
}

// ショット描画
void DrawShot(Shot& shot)
{
	// 自機の弾iの移動ルーチン( 存在状態を保持している変数の内容がtrue(存在する)の場合のみ行う )
	if (shot.visibleFlag == true)
	{
		// 弾iを１６ドット上に移動させる
		shot.Y -= 16;

		// 画面外に出てしまった場合は存在状態を保持している変数にfalse(存在しない)を代入する
		if (shot.Y < 0 - shot.H)
		{
			shot.visibleFlag = false;
		}

		// 画面に弾iを描画する
 		DrawGraph(shot.X, shot.Y, shot.Graph, FALSE);
	}
}

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

	Enemy enemy;
	Player player;
	Shot shot[SHOT];

	// グラフィックの描画先を裏画面にセット
	SetDrawScreen(DX_SCREEN_BACK);

	InitializePlayer(player);	// プレイヤー初期化

	// エネミーのグラフィックをメモリにロード＆表示座標を初期化
	InitializeEnemy(enemy);

	for (int i = 0; i < SHOT; i++)
	{
		InitializeShot(shot[i]);	// 弾初期化
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
		for (int i = 0;  i  < SHOT;  i ++)
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