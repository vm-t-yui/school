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
	void Init();		// 初期化
	void Update();		// アップデート
	void Draw();		// 描画

	void OnDamage();	// ダメージを受けた

	int GetLife() { return Life; }
	int GetX() { return X; }
	int GetY() { return Y; }
	int GetW() { return W; }
	int GetH() { return H; }

private:
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
};
