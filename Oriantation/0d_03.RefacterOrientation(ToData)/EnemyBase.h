#pragma once

/// <summary>
/// エネミーの基底になるクラス
/// </summary>
class EnemyBase
{
public:
	// 敵初期化用データ
	struct Data
	{
		const char* graphPath;
		int			firstX;
		int			firstY;
		int			life;
		int			speed;
		int			speedUpLifeLimit;
	};

	int GetLife() const { return life; }
	int GetX() const { return x; }
	int GetY() const { return y; }
	int GetW() const { return w; }
	int GetH() const { return h; }

	void Draw() const;	// 描画
	void OnDamage();	// ダメージを受けた

	void Init(const Data& srcData);		// 初期化
	virtual void Update();		// アップデート

protected:
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

	// エネミーが右移動しているかどうか
	bool	rightMove;

	const Data* data;	// データへの参照
};

