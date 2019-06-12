#include "Enemy.h"

//------------------------------//
// エネミーの初期化.
//------------------------------//
void InitEnemy(Enemy& enemy)
{
	// エネミーのグラフィックをメモリにロード＆表示座標を初期化
	enemy.graph = LoadGraph("data/texture/enemy.png");
	enemy.x = 0;
	enemy.y = 50;

	// エネミーのグラフィックのサイズを得る
	int w, h;
	GetGraphSize(enemy.graph, &w, &h);
	enemy.w = (float)w;
	enemy.h = (float)h;
}

//------------------------------//
// エネミーの更新.
//------------------------------//
void UpdateEnemy(Enemy& enemy)
{
	++enemy.x;

	if (enemy.x < 0)
	{
		enemy.x = 0;
	}
	if (enemy.x > SCREEN_WIDTH - enemy.w)
	{
		enemy.x = SCREEN_WIDTH - enemy.w;
	}
	if (enemy.y < 0)
	{
		enemy.y = 0;
	}
	if (enemy.y > SCREEN_HEIGHT - enemy.h)
	{
		enemy.y = SCREEN_HEIGHT - enemy.h;
	}
}

//------------------------------//
// エネミーの描画.
//------------------------------//
void DrawEnemy(Enemy& enemy)
{
	DrawGraph((int)enemy.x, (int)enemy.y, enemy.graph, TRUE);
}