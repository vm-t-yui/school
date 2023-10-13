//-----------------------------------------------------------------------------
// @brief  エネミー処理.
//-----------------------------------------------------------------------------

#pragma once

#include "DxLib.h"
#include "Common.h"

class EnemyParamUI;
template <class TState> class StateMachine;

// エネミークラス.
class Enemy
{
public:
	int		X;
	int		Y;
	int		Graph;
	bool	DamageFlag;
	int		DamageCounter;
	int		W;
	int		H;

	// エネミーが右移動しているかどうか
	bool	RightMove;

	// 関数宣言.
	void Init(OnDamageDelegate onDamageDelegate);	// 初期化.
	void Update();			// アップデート.
	void EnterNormal();		// 通常Enter
	void UpdateNormal();	// 通常Update
	void ExitNormal();		// 通常Exit
	void EnterDamage();		// ダメージEnter
	void UpdateDamage();	// ダメージUpdate
	void ExitDamage();		// ダメージExit
	void Draw(const EnemyParam& param);		// 描画.
	void OnDamage(int damage);

	// ステート
	enum State
	{
		Normal,
		Damage
	};

private:
	OnDamageDelegate onDamage;
	StateMachine<State> stateMachine;
};
