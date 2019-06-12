#pragma once

#include "shot.h"

// プレイヤー構造体.
struct Player
{
	int Graph;
	int X;
	int Y;
	int W;
	int H;
	int ShotIntervalCount;
};

// プレイヤー関数群.
void InitPlayer(Player& player);					// 初期化.
void UpdatePlayer(Player& player, Shot* shot);		// アップデート.
void DrawPlayer(Player& player);					// 描画.