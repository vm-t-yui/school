// 2023 Takeru Yui All Rights Reserved.
#include "DxLib.h"
#include "Enemy.h"

/// <summary>
/// 初期化
/// </summary>
void Enemy::Init()
{
	// エネミーのグラフィックをメモリにロード＆表示座標を初期化
	char* enemyGlaphStr = "data/texture/EpicEnemy.png";
	graph = LoadGraph(enemyGlaphStr);
	damageGraph = LoadGraph(enemyGlaphStr);
	GraphFilter(damageGraph, DX_GRAPH_FILTER_HSB, 0, 0, 0, 256);
	x = 0;
	y = 50;
	life = ENEMY_LIFE;

	// エネミーが顔を歪めているかどうかの変数に『歪めていない』を表すFALSEを代入
	damageFlag = false;

	// エネミーのグラフィックのサイズを得る
	GetGraphSize(graph, &w, &h);

	rightMove = true;
}

/// <summary>
/// 更新
/// </summary>
void Enemy::Update()
{
	// エネミーの座標を移動している方向に移動する
	if (rightMove == true)
	{
		x += 3;
	}
	else
	{
		x -= 3;
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