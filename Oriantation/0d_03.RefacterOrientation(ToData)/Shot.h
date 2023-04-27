// 2023 Takeru Yui All Rights Reserved.
#pragma once

#include "DxLib.h"
#include "Common.h"
#include <vector>

class EnemyBase;

/// <summary>
/// ショット
/// </summary>
class Shot
{
public:
	void Init();														// 初期化
	void Update(const std::vector<EnemyBase*> enemies);					// アップデート
	void Draw();														// 描画

	void OnShot(int playerX, int playerY);								// ショットうたれた

	bool GetVisibleFlag() { return visibleFlag; }

private:
	int		graph;
	int		x;
	int		y;
	bool	visibleFlag;
	int		w;
	int		h;
};