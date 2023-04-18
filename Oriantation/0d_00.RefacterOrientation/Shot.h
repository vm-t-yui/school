// 2023 Takeru Yui All Rights Reserved.
#pragma once

#include "DxLib.h"
#include "Common.h"
#include "Enemy.h"
#include "Enemy2.h"
#include "Enemy3.h"

/// <summary>
/// ショット
/// </summary>
class Shot
{
public:
	void Init();														// 初期化
	void Update(Enemy& enemy, Enemy2& enemy2, Enemy3& enemy3);			// アップデート
	void Draw();														// 描画

	void OnShot(int playerX, int playerY);								// ショットうたれた

	bool GetVisibleFlag() { return VisibleFlag; }

private:
	int		Graph;
	int		X;
	int		Y;
	bool	VisibleFlag;
	int		W;
	int		H;
};