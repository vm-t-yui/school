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

	int		modelHandle;
	int		currentAnimAttachIndex;		// アタッチしたアニメーションのインデックス
	float	currentAnimTotalTime;		// 再生中のアニメーションのトータル時間
	float	currentAnimTimeCount;		// 再生中のアニメーションの現在時間

	constexpr static float		Speed		= 3.0f;
	constexpr static float		HitSize		= 30;	// エネミーの当たり判定サイズ
	constexpr static int		DamageTime	= 30;	// エネミーのダメージ顔になっている時間
	inline const static VECTOR	FirstPos	= VGet(0, 50, 0);

	// モデルの表示スケールと位置補正
	inline const static VECTOR	ModelScale	= VGet(0.003f, 0.003f, 0.003f);
	inline const static VECTOR	ModelFixPos	= VGet(0.0f, -0.25f, 0.0f);

	constexpr static int	IdleAnimationIndex		= 2;	// エネミーの待機のインデックス
	constexpr static int	DamageAnimationIndex	= 4;	// エネミーのダメージのインデックス
	constexpr static float	AnimationSpeed			= 0.5f;	// エネミーのダメージのインデックス

	void Initialize();
	void Update();
	void Draw() const;
};