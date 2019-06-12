#include "Player.h"

//------------------------------//
// プレイヤー初期化.
//------------------------------//
void InitPlayer(Player& player)
{
	// プレイヤーのグラフィックをメモリにロード＆表示座標を初期化
	player.obj.graph = LoadGraph("data/texture/player.png");
	player.obj.pos.x = 288;
	player.obj.pos.y = 400;
	player.obj.dir = VGet(0,0,0);
	player.speed = 3.45f;

	// プレイヤーの画像のサイズを得る
	int w, h;
	GetGraphSize(player.obj.graph, &w, &h);
	player.obj.w = (float)w;
	player.obj.h = (float)h;
}

//------------------------------//
// プレイヤー更新.
//------------------------------//
void UpdatePlayer(Player& player)
{
	// 矢印キーを押していたらプレイヤーを移動させる
	float x = 0;
	float y = 0;
	if (CheckHitKey(KEY_INPUT_UP) == 1)
	{
		y = -1;
	}
	if (CheckHitKey(KEY_INPUT_DOWN) == 1)
	{
		y = 1;
	}
	if (CheckHitKey(KEY_INPUT_LEFT) == 1)
	{
		x = -1;
	}
	if (CheckHitKey(KEY_INPUT_RIGHT) == 1)
	{
		x = 1;
	}
	player.obj.dir = VGet(x, y, 0);

	if (VSquareSize(player.obj.dir) != 0)
	{
		player.obj.dir = VNorm(player.obj.dir);
	}

	VECTOR velocity = VScale(player.obj.dir, player.speed);
	player.obj.pos = VAdd(player.obj.pos, velocity);

	// プレイヤーが画面端からはみ出そうになっていたら画面内の座標に戻してあげる
	if (player.obj.pos.x < 0)
	{
		player.obj.pos.x = 0;
	}
	if (player.obj.pos.x > SCREEN_WIDTH - player.obj.w)
	{
		player.obj.pos.x = SCREEN_WIDTH - player.obj.w;
	}
	if (player.obj.pos.y < 0)
	{
		player.obj.pos.y = 0;
	}
	if (player.obj.pos.y > SCREEN_HEIGHT - player.obj.h)
	{
		player.obj.pos.y = SCREEN_HEIGHT - player.obj.h;
	}
}

//------------------------------//
// プレイヤーを描画.
//------------------------------//
void DrawPlayer(Player& player)
{
	DrawGraph((int)player.obj.pos.x, (int)player.obj.pos.y, player.obj.graph, TRUE);
}

