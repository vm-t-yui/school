#pragma once

#include "enemy.h"

// ショット.
struct Shot
{
	int		Graph;
	int		X;
	int		Y;
	bool	VisibleFlag;
	int		W;
	int		H;
};

// ショット関数群.
void InitShot(Shot& shot);					// 初期化.
void UpdateShot(Shot& shot, Enemy& enemy);	// アップデート.
void DrawShot(Shot& shot);					// 描画.
