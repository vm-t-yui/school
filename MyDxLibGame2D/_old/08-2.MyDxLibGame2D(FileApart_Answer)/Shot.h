#pragma once

// ショットの数.
#define SHOT 20

// ショット構造体.
struct Shot
{
	int		Graph;
	int		X;
	int		Y;
	bool	visibleFlag;
	int		W;
	int		H;
};

// プロトタイプ宣言
struct Enemy;

void InitializeShot(Shot& shot, int shotGraph);							// ショット初期化
void UpdateShot(Shot& shot, Enemy& enemy);								// ショット更新
void DrawShot(Shot& shot);												// ショット描画