//-----------------------------------------------------------------------------
// @brief  エネミー処理.
//-----------------------------------------------------------------------------

#pragma once

#include "DxLib.h"
#include "Common.h"
#include "EnemyUiHp.h"
#include "EnemyParamUI.h"

// エネミークラス.
class Enemy
{
public:
	int		X;
	int		Y;
	int		Graph;
	bool	DamageFlag;
	int		DamageCounter;
	int		DamageGraph;
	int		W;
	int		H;

	// エネミーが右移動しているかどうか
	bool	RightMove;

	// 関数宣言.
	void Init();	// 初期化.
	void Update(EnemyParamUI& enemyParamUI);	// アップデート.
	void Draw(const EnemyParam& param);	// 描画.
};
