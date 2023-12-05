#pragma once

// エネミー構造体.
struct Enemy
{
	int		X;
	int		Y;
	int		Graph;
	bool	DamageFlag;
	int		DamageCounter;
	int		DamageGraph;
	int		W;
	int		H;
	bool    RightMove;
};

void InitializeEnemy(Enemy& enemy);										// エネミー初期化
void UpdateEnemy(Enemy& enemy);											// エネミー更新
void DrawEnemy(Enemy& enemy);											// エネミー描画
