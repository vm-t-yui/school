//-----------------------------------------------------------------------------
// @brief  プレイヤー処理.
//-----------------------------------------------------------------------------

#pragma once

#include "DxLib.h"
#include "Common.h"
#include "Shot.h"

// プレイヤー.
struct Player
{
	int Graph;
	int X;
	int Y;
	int W;
	int H;

	// ショットのインターバルを設定するカウンター
	int shotIntervalCount;
};
//----------------------------//
// プレイヤー関数群.
//----------------------------//
// 初期化.
void InitPlayer(Player& player);
// アップデート.
void UpdatePlayer(Player& player, Shot* shot);
// 描画.
void DrawPlayer(Player& player);