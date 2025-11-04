#pragma once

/// <summary>
/// エネミークラス
/// </summary>
class Enemy
{
public:
	// 敵の状態を表す列挙体
	enum class State : int
	{
		Normal,		// 通常
		Damage		// ダメージ
	};

	VECTOR	pos;
	VECTOR	dir;	// エネミーの向き
	int		graphNormal;
	int		graphDamage;
	int		graph;
	int		w;
	int		h;
	bool	isRightMove;
	State	state;
	int		damageCount;

	constexpr static float		Speed = 3.0f;
	constexpr static float		HitSize = 30;	// エネミーの当たり判定サイズ
	constexpr static int		DamageTime = 30;	// エネミーのダメージ顔になっている時間
	inline const static VECTOR	FirstPos = VGet(0, 50, 0);

	void Initialize();
	void Update();
	void Draw() const;
};