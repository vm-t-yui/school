#pragma once

#include "Common.h"

struct Player;

// エネミー構造体.
struct Enemy
{
	GameObject	obj;
	float		speed;
};

// プロトタイプ宣言.
void InitEnemy(Enemy& enemy);
void UpdateEnemy(Enemy& enemy, Player& player);
void DrawEnemy(Enemy& enemy);