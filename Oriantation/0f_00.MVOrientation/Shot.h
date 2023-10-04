//-----------------------------------------------------------------------------
// @brief  ショット処理.
//-----------------------------------------------------------------------------

#pragma once

#include "DxLib.h"
#include "Common.h"

class Enemy;
class EnemyParam;

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
	void Update(Enemy& enemy, const EnemyParam& param);	// アップデート.
	void Draw();				// 描画.
};