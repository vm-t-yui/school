#include "Common.h"
#include "Enemy.h"
#include "player.h"

//------------------------------//
// エネミーの初期化.
//------------------------------//
void InitEnemy(Enemy& enemy)
{
	// エネミーのグラフィックをメモリにロード＆表示座標を初期化
	enemy.obj.graph = LoadGraph("data/texture/enemy.png");
	enemy.obj.pos.x = 320.0f;
	enemy.obj.pos.y = 50;

	// エネミーのグラフィックのサイズを得る
	int w, h;
	GetGraphSize(enemy.obj.graph, &w, &h);
	enemy.obj.w = (float)w;
	enemy.obj.h = (float)h;

	enemy.speed = 1.0f;
}

//------------------------------//
// エネミーの更新.
//------------------------------//
void UpdateEnemy(Enemy& enemy)
{
	// 任意の角度の方向に移動する.
	static float degree = 100.0f;
	static float radian = 0.0f;
	degree += 1.0f;

	// degreeをradianに変換.
	radian = degree * DEG_TO_RAD;
	printfDx("degree:%f ", degree);
	printfDx("radian:%f\n", radian);
	enemy.obj.dir.x = cosf(radian);
	enemy.obj.dir.y = -sinf(radian);

	// 向きにスピードをかけて、移動力に変更する.
	Vector velocity = ScaleVector(enemy.obj.dir, enemy.speed);

	// ポジションに移動力を足す.
	enemy.obj.pos = AddVector(enemy.obj.pos, velocity);


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