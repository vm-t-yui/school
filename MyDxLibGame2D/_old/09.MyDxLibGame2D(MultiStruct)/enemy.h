#pragma once

#include "Common.h"

// エネミー構造体.
struct Enemy
{
	GameObject obj;
};

// プロトタイプ宣言.
void InitEnemy(Enemy& enemy);
void UpdateEnemy(Enemy& enemy);
void DrawEnemy(Enemy& enemy);