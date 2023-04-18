// 2023 Takeru Yui All Rights Reserved.
#pragma once

#include "DxLib.h"
#include "Common.h"

/// <summary>
/// 遅いエネミー
/// </summary>
class Enemy2
{
public:
	void Init();		// 初期化
	void Update();		// アップデート
	void Draw();		// 描画

	void OnDamage();	// ダメージを受けた

	int GetLife() { return life; }
	int GetX() { return x; }
	int GetY() { return y; }
	int GetW() { return w; }
	int GetH() { return h; }

private:
	int		x;
	int		y;
	int		graph;
	bool	damageFlag;
	int		damageCounter;
	int		damageGraph;
	int		w;
	int		h;
	int		life;

	// エネミーが右移動しているかどうか
	bool	rightMove;
};
