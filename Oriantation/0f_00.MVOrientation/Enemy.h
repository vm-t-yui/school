//-----------------------------------------------------------------------------
// @brief  エネミー処理.
//-----------------------------------------------------------------------------

#pragma once

#include "DxLib.h"
#include "Common.h"

class EnemyParamUI;

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
	void Init(EnemyParamUI& enemyParamUI);	// 初期化.
	void Update();	// アップデート.
	void Draw(const EnemyParam& param);		// 描画.
	void OnDamage(int damage);

private:
	EnemyParamUI* paramUI = nullptr;
};
