#pragma once

#include "Common.h"

// プレイヤー構造体.
struct Player
{
	int		graph;
	float	x;
	float	y;
	float	speed;
	float	w;
	float	h;
};

// プロトタイプ宣言.
void InitPlayer(Player& player);
void UpdatePlayer(Player& player);
void DrawPlayer(Player& player);
