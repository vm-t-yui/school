//-----------------------------------------------------------------------------
// @brief  ショット処理.
//-----------------------------------------------------------------------------

#pragma once

class Enemy;

// ショット.
class Shot
{
public:
	void Init();				// 初期化.
	void Update(Enemy& enemy);	// アップデート.
	void Draw();				// 描画.

	int GetX()		const { return X; }
	int GetY()		const { return Y; }
	int GetW()		const { return W; }
	int GetH()		const { return H; }

	// boolのGetterはIs●●という形にする
	bool IsVisible() const { return isVisible; }

	// 弾が撃たれたとき
	void OnShoot(int shooterX, int shooterY, int shooterW, int shooterH);

private:
	int		Graph;
	int		X;
	int		Y;
	bool	isVisible;		// VisibleFlagはboolの時点でFlagの意味を持つので改名。「見えているか」
	int		W;
	int		H;
};