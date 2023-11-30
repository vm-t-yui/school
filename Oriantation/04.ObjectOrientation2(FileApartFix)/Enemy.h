//-----------------------------------------------------------------------------
// @brief  エネミー処理.
//-----------------------------------------------------------------------------

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
	int		Life;

	// エネミーが右移動しているかどうか
	bool	RightMove;
};

//----------------------------//
// エネミー関数群プロトタイプ宣言.
//----------------------------//
// 初期化.
void InitEnemy(Enemy& enemy);

// アップデート.
void UpdateEnemy(Enemy& enemy);

// 描画.
void DrawEnemy(Enemy& enemy);
