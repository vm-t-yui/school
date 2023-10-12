//-----------------------------------------------------------------------------
// @brief  プレイヤー処理.
//-----------------------------------------------------------------------------

#pragma once

#include "DxLib.h"
#include "Common.h"
#include "Shot.h"

// プレイヤー.
class Player
{
public:
	int Graph;
	int X;
	int Y;
	int W;
	int H;

	// ショットのインターバルを設定するカウンター
	int shotIntervalCount;

	void Init();				// 初期化.
	void Update(Shot* shot);	// アップデート.
	void Draw();				// 描画.
};