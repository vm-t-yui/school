//-----------------------------------------------------------------------------
// @brief  ショット処理.
//-----------------------------------------------------------------------------

#pragma once

struct Enemy;

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
//----------------------------//
// ショット関数群.
//----------------------------//
// 初期化.
void InitShot(Shot& shot);
// アップデート.
void UpdateShot(Shot& shot, Enemy& enemy);
// 描画.
void DrawShot(Shot& shot);