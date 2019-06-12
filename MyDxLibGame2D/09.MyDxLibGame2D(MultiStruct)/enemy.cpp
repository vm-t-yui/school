#include "Enemy.h"

//------------------------------//
// エネミーの初期化.
//------------------------------//
void InitEnemy(Enemy& enemy)
{
	// エネミーのグラフィックをメモリにロード＆表示座標を初期化
	enemy.obj.graph = LoadGraph("data/texture/enemy.png");
	enemy.obj.x = 0;
	enemy.obj.y = 50;

	// エネミーのグラフィックのサイズを得る
	int w, h;
	GetGraphSize(enemy.obj.graph, &w, &h);
	enemy.obj.w = (float)w;
	enemy.obj.h = (float)h;
}

//------------------------------//
// エネミーの更新.
//------------------------------//
void UpdateEnemy(Enemy& enemy)
{
	++enemy.obj.x;

	if (enemy.obj.x < 0)
	{
		enemy.obj.x = 0;
	}
	if (enemy.obj.x > SCREEN_WIDTH - enemy.obj.w)
	{
		enemy.obj.x = SCREEN_WIDTH - enemy.obj.w;
	}
	if (enemy.obj.y < 0)
	{
		enemy.obj.y = 0;
	}
	if (enemy.obj.y > SCREEN_HEIGHT - enemy.obj.h)
	{
		enemy.obj.y = SCREEN_HEIGHT - enemy.obj.h;
	}
}

//------------------------------//
// エネミーの描画.
//------------------------------//
void DrawEnemy(Enemy& enemy)
{
	DrawGraph((int)enemy.obj.x, (int)enemy.obj.y, enemy.obj.graph, TRUE);
}