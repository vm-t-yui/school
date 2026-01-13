#pragma once

/// <summary>
/// エネミークラス
/// </summary>
class Enemy
{
public:
	constexpr static float		HitSize		= 30;	// エネミーの当たり判定サイズ

	Enemy();
	virtual ~Enemy() { /* 処理なし */ }

	void Initialize();
	void Update();
	void Draw() const;
	void OnDamage();

	VECTOR GetPos() const { return pos; }

private:
	// 敵の状態を表す列挙体
	enum class State : int
	{
		Normal,		// 通常
		Damage		// ダメージ
	};

	int		modelHandle;
	int		currentAnimAttachIndex;		// アタッチしたアニメーションのインデックス
	float	currentAnimTotalTime;		// 再生中のアニメーションのトータル時間
	float	currentAnimTimeCount;		// 再生中のアニメーションの現在時間
	float	currentAnimSpeed;			// 再生中のアニメーションスピード
	bool	currentAnimIsLoop;			// ループするかどうか

	VECTOR	pos;
	VECTOR	dir;	// エネミーの向き
	bool	isUpMove;
	State	state;
	int		damageCount;

	void ChangeAnimation(int animationIndex, float speed, bool isLoop);	// アニメーションの切り替え
};