#pragma once

class Enemy; // プロトタイプ宣言

/// <summary>
/// ショットクラス
/// </summary>
class Shot
{
public:
	constexpr static int	Num			= 5;

	Shot();
	virtual ~Shot() { /* 処理なし */ }
	void Initialize();
	void Update(Enemy& enemy);
	void Draw() const;

	void OnShoot(const VECTOR& shootPos);

	VECTOR	GetPos()	const { return pos; }
	bool	IsAlive()	const { return isAlive; }

private:
	VECTOR	pos;
	VECTOR	dir;
	bool	isAlive;

	int		modelHandle;
};