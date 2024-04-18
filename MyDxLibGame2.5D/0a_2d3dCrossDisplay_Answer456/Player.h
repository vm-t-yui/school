#pragma once

#include "Common.h"

// プレイヤー構造体.
struct Player
{
	GameObject	obj;
	float		speed;
};

// プロトタイプ宣言.
void InitPlayer(Player& player);
void UpdatePlayer(Player& player);
void DrawPlayer(Player& player);
