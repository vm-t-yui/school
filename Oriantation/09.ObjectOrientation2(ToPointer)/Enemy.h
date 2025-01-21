//-----------------------------------------------------------------------------
// @brief  エネミー処理.
//-----------------------------------------------------------------------------

#pragma once

class Enemy;

// エネミークラス.
class Enemy
{
public:
	// 関数宣言.
	void Init();		// 初期化.
	void Update();		// アップデート.
	void Draw();		// 描画.
	void OnDamage();	// ダメージを受けた時

	int GetX()		const { return X; }
	int GetY()		const { return Y; }
	int GetW()		const { return W; }
	int GetH()		const { return H; }
	int GetLife()	const { return Life; }

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
