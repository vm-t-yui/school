#include "DxLib.h"
#include "Enemy.h"

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
