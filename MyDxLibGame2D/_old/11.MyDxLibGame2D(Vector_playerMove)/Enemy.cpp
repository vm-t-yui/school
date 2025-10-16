#include "Enemy.h"

//------------------------------//
// エネミーの初期化.
//------------------------------//
void InitEnemy(Enemy& enemy)
{
	// エネミーのグラフィックをメモリにロード＆表示座標を初期化
	enemy.obj.graph = LoadGraph("data/texture/enemy.png");
	enemy.obj.pos.x = 0;
	enemy.obj.pos.y = 50;

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
	++enemy.obj.pos.x;

	if (enemy.obj.pos.x < 0)
	{
		enemy.obj.pos.x = 0;
	}
	if (enemy.obj.pos.x > SCREEN_WIDTH - enemy.obj.w)
	{
		enemy.obj.pos.x = SCREEN_WIDTH - enemy.obj.w;
	}
	if (enemy.obj.pos.y < 0)
	{
		enemy.obj.pos.y = 0;
	}
	if (enemy.obj.pos.y > SCREEN_HEIGHT - enemy.obj.h)
	{
		enemy.obj.pos.y = SCREEN_HEIGHT - enemy.obj.h;
	}
}

//------------------------------//
// エネミーの描画.
//------------------------------//
void DrawEnemy(Enemy& enemy)
{
	DrawGraph((int)enemy.obj.pos.x, (int)enemy.obj.pos.y, enemy.obj.graph, TRUE);
}