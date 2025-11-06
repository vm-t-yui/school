#pragma once

class Enemy; // プロトタイプ宣言

/// <summary>
/// ショットクラス
/// </summary>
class Shot
{
public:
	int graph;
	int w;
	int h;

	VECTOR	pos;
	VECTOR	dir;
	bool	isAlive;

	int modelHandle;

	constexpr static int	Num			= 3;
	constexpr static float	Speed		= 3.0f;
	constexpr static float	AliveLimitY	= -80.0f;
	constexpr static float	HitSize		= 10;		// ショットの当たり判定サイズ

	inline const static VECTOR	ModelScale	= VGet(0.002f, 0.002f, 0.002f);

	void Initialize();
	void Update(Enemy& enemy);
	void Draw() const;
};