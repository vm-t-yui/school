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
	player.speed = 10;

	// プレイヤーの画像のサイズを得る
	int w, h;
	GetGraphSize(player.obj.graph, &w, &h);
	player.obj.w = (float)w;
	player.obj.h = (float)h;

	player.obj.dir.x = 0;
	player.obj.dir.y = 0;
}

//------------------------------//
// プレイヤー更新.
//------------------------------//
void UpdatePlayer(Player& player)
{
	// 矢印キーを押していたらプレイヤーを移動させる
	if (CheckHitKey(KEY_INPUT_UP) == 1)
	{
		player.obj.pos.y -= player.speed;
	}
	if (CheckHitKey(KEY_INPUT_DOWN) == 1)
	{
		player.obj.pos.y += player.speed;
	}
	if (CheckHitKey(KEY_INPUT_LEFT) == 1)
	{
		player.obj.pos.x -= player.speed;
	}
	if (CheckHitKey(KEY_INPUT_RIGHT) == 1)
	{
		player.obj.pos.x += player.speed;
	}

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

