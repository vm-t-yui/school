#pragma once

class Enemy; // プロトタイプ宣言

/// <summary>
/// ショットクラス
/// </summary>
class Shot
{
public:
	VECTOR	pos;
	VECTOR	dir;
	bool	isAlive;

	int modelHandle;

	constexpr static int	Num			= 5;
	constexpr static float	Speed		= 30.0f;
	constexpr static float	AliveLimitX	= Graphics::ScreenW + 40.0f;
	constexpr static float	HitSize		= 10;		// ショットの当たり判定サイズ

	inline const static VECTOR	ModelScale	= VGet(0.002f, 0.002f, 0.002f);

	void Initialize();
	void Update(Enemy& enemy);
	void Draw() const;
};