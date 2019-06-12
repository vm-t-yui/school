//-----------------------------------------------------------------------------
// @brief  エネミー処理.
//-----------------------------------------------------------------------------
#include"enemy.h"

//----------------------------//
// 初期化.
//----------------------------//
void InitEnemy(Enemy& enemy)
{
	// エネミーのグラフィックをメモリにロード＆表示座標を初期化
	char* enemyGlaphStr = "data/texture/EpicEnemy.png";
	enemy.Graph = LoadGraph(enemyGlaphStr);
	enemy.DamageGraph = LoadGraph(enemyGlaphStr);
	GraphFilter(enemy.DamageGraph, DX_GRAPH_FILTER_HSB, 0, 0, 0, 256);
	enemy.X = 0;
	enemy.Y = 50;
	enemy.Life = ENEMY_LIFE;

	// エネミーが顔を歪めているかどうかの変数に『歪めていない』を表すFALSEを代入
	enemy.DamageFlag = false;

	// エネミーのグラフィックのサイズを得る
	GetGraphSize(enemy.Graph, &enemy.W, &enemy.H);

	enemy.RightMove = true;
}

//----------------------------//
// アップデート.
//----------------------------//
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
	if (enemy.X > SCREEN_W - enemy.W)
	{
		enemy.X = SCREEN_W - enemy.W;
		enemy.RightMove = false;
	}
	else if (enemy.X < 0)
	{
		enemy.X = 0;
		enemy.RightMove = true;
	}

	// エネミーを描画
	// ダメージを受けているかどうかで処理を分岐
	if (enemy.DamageFlag == true)
	{
		enemy.DamageCounter++;

		if (enemy.DamageCounter == 5)
		{
			// 『ダメージをうけていない』を表すFALSEを代入
			enemy.DamageFlag = false;
		}
	}
}

//----------------------------//
// 描画.
//----------------------------//
void DrawEnemy(Enemy& enemy)
{
	if (enemy.Life > 0)
	{
		// ダメージを受けている場合はダメージ時のグラフィックを描画する
		if (enemy.DamageFlag == true)
		{
			DrawGraph(enemy.X, enemy.Y, enemy.DamageGraph, TRUE);
		}
		else
		{
			DrawGraph(enemy.X, enemy.Y, enemy.Graph, TRUE);
		}
	}
}