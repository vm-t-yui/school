// 2023 Takeru Yui All Rights Reserved.
#pragma once

#include "Common.h"

/// <summary>
/// 普通のエネミー
/// </summary>
class Enemy
{
public:
	void Init();		// 初期化
	void Update();		// アップデート
	void Draw() const ;		// 描画

	void OnDamage();	// ダメージを受けた

	int GetLife() const { return life; }
	int GetX() const { return x; }
	int GetY() const { return y; }
	int GetW() const { return w; }
	int GetH() const { return h; }

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
