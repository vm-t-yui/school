#include "Player.h"

//------------------------------//
// プレイヤー初期化.
//------------------------------//
void InitPlayer(Player& player)
{
	// プレイヤーのグラフィックをメモリにロード＆表示座標を初期化
	player.graph = LoadGraph("data/texture/player.png");
	player.x = 288;
	player.y = 400;
	player.speed = 3;

	// プレイヤーの画像のサイズを得る
	int w, h;
	GetGraphSize(player.graph, &w, &h);
	player.w = (float)w;
	player.h = (float)h;
}

//------------------------------//
// プレイヤー更新.
//------------------------------//
void UpdatePlayer(Player& player)
{
	// 矢印キーを押していたらプレイヤーを移動させる
	if (CheckHitKey(KEY_INPUT_UP) == 1)
	{
		player.y -= player.speed;
	}
	if (CheckHitKey(KEY_INPUT_DOWN) == 1)
	{
		player.y += player.speed;
	}
	if (CheckHitKey(KEY_INPUT_LEFT) == 1)
	{
		player.x -= player.speed;
	}
	if (CheckHitKey(KEY_INPUT_RIGHT) == 1)
	{
		player.x += player.speed;
	}

	// プレイヤーが画面端からはみ出そうになっていたら画面内の座標に戻してあげる
	if (player.x < 0)
	{
		player.x = 0;
	}
	if (player.x > SCREEN_WIDTH - player.w)
	{
		player.x = SCREEN_WIDTH - player.w;
	}
	if (player.y < 0)
	{
		player.y = 0;
	}
	if (player.y > SCREEN_HEIGHT - player.h)
	{
		player.y = SCREEN_HEIGHT - player.h;
	}
}

//------------------------------//
// プレイヤーを描画.
//------------------------------//
void DrawPlayer(Player& player)
{
	DrawGraph((int)player.x, (int)player.y, player.graph, TRUE);
}

