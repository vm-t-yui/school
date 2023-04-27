#pragma once

/// <summary>
/// エネミーの基底になるクラス
/// </summary>
class EnemyBase
{
public:
	int GetLife() const { return life; }
	int GetX() const { return x; }
	int GetY() const { return y; }
	int GetW() const { return w; }
	int GetH() const { return h; }

	void Draw() const;	// 描画
	void OnDamage();	// ダメージを受けた

	virtual void Init();		// 初期化
	virtual void Update();		// アップデート

protected:
	virtual void SetInitializeData() = 0;
	virtual void Move();

	int		x;
	int		y;
	int		graph;
	bool	damageFlag;
	int		damageCounter;
	int		damageGraph;
	int		w;
	int		h;
	int		life;
	int     speed;
	int		speedUpLifeLimit;
	const char* enemyGlaphStr;

	// エネミーが右移動しているかどうか
	bool	rightMove;
};

