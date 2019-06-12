#pragma once

#include "Common.h"

// エネミー構造体.
struct Enemy
{
	float	x;
	float	y;
	int		graph;
	float	w;
	float	h;
};

// プロトタイプ宣言.
void InitEnemy(Enemy& enemy);
void UpdateEnemy(Enemy& enemy);
void DrawEnemy(Enemy& enemy);