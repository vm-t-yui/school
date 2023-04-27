// 2023 Takeru Yui All Rights Reserved.
#pragma once

#include "DxLib.h"
#include "Common.h"
#include "Shot.h"

/// <summary>
/// プレイヤー
/// </summary>
class Player
{
public:
	void Init();				// 初期化
	void Update(Shot shot[], int shotNum);	// アップデート
	void Draw();				// 描画

private:
	int graph;
	int x;
	int y;
	int w;
	int h;

	// ショットのインターバルを設定するカウンター
	int shotIntervalCount;
};