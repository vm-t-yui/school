// 2023 Takeru Yui All Rights Reserved.
#include "DxLib.h"
#include "Enemy3.h"

/// <summary>
/// 初期化
/// </summary>
void Enemy3::Init()
{
	// エネミーのグラフィックをメモリにロード＆表示座標を初期化
	char* enemyGlaphStr = "data/texture/EpicEnemy3.png";
	graph = LoadGraph(enemyGlaphStr);
	damageGraph = LoadGraph(enemyGlaphStr);
	GraphFilter(damageGraph, DX_GRAPH_FILTER_HSB, 0, 0, 0, 256);
	x = 0;
	y = 150;
	life = ENEMY_LIFE3;

	// エネミーが顔を歪めているかどうかの変数に『歪めていない』を表すFALSEを代入
	damageFlag = false;

	// エネミーのグラフィックのサイズを得る
	GetGraphSize(graph, &w, &h);

	rightMove = true;
}

/// <summary>
/// 更新
/// </summary>
void Enemy3::Update()
{
	// エネミーの座標を移動している方向に移動する
	if (rightMove == true)
	{
		if (life < 10)
		{
			x += 8;
		}
		else
		{
			x += 4;
		}
	}
	else
	{
		if (life < 10)
		{
			x -= 8;
		}
		else
		{
			x -= 4;
		}
	}

	// エネミーが画面端からでそうになっていたら画面内の座標に戻してあげ、移動する方向も反転する
	if (x > SCREEN_W - w)
	{
		x = SCREEN_W - w;
		rightMove = false;
	}
	else if (x < 0)
	{
		x = 0;
		rightMove = true;
	}

	// ダメージを受けているかどうかで処理を分岐
	if (damageFlag == true)
	{
		damageCounter++;

		if (damageCounter == 5)
		{
			// 『ダメージをうけていない』を表すFALSEを代入
			damageFlag = false;
		}
	}
}
