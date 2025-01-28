//-----------------------------------------------------------------------------
// @brief  ショット処理.
//-----------------------------------------------------------------------------

#pragma once

#include <memory>

// リストを使うかどうか。使うなら1、使わないなら0
#define USE_LIST 1

#if USE_LIST
	#include <list>
#else
	#include <vector>
#endif

// ショットのコンテナ型を作る
class Shot;
#if USE_LIST
	typedef std::list<std::shared_ptr<Shot>> ShotContainer;
#else
	typedef std::vector<std::shared_ptr<Shot>> ShotContainer;
#endif

#define SHOT			3		// ショットの最大数
#define SHOT_SPEED		2		// 通常ショットのスピード
#define SHOT_INTERVAL	3		// 通常ショットの発射インターバル

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