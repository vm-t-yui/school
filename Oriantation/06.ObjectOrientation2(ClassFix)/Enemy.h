//-----------------------------------------------------------------------------
// @brief  エネミー処理.
//-----------------------------------------------------------------------------

#pragma once

#include "DxLib.h"
#include "Common.h"

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
	int		Life;

	// エネミーが右移動しているかどうか
	bool	RightMove;

	// 関数宣言.
	void Init();	// 初期化.
	void Update();	// アップデート.
	void Draw();	// 描画.
};
