//-----------------------------------------------------------------------------
// @brief  ショット処理.
//-----------------------------------------------------------------------------

#pragma once

class Enemy;

// ショット.
class Shot
{
public:
	int		Graph;
	int		X;
	int		Y;
	bool	VisibleFlag;
	int		W;
	int		H;

	void Init();				// 初期化.
	void Update(Enemy& enemy);	// アップデート.
	void Draw();				// 描画.
};