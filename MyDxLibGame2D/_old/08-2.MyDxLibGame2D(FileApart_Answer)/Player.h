#pragma once

// プレイヤー構造体.
struct Player
{
	int Graph;
	int X;
	int Y;
	int W;
	int H;
	bool PrevShotFlag;
};

// プロトタイプ宣言
struct Shot;

void InitializePlayer(Player& player);									// プレイヤー初期化
void UpdatePlayer(Player& player, Shot shotArray[], int shotArraySize);	// プレイヤーの更新処理
void DrawPlayer(Player& player);										// プレイヤーの描画処理
