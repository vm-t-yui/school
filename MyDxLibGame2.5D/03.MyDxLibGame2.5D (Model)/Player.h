//-----------------------------------------------------------------------------
// @brief  プレイヤークラス.
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#ifndef _PLAYER_H_
#define _PLAYER_H_

#define USE_MODEL 0

#include "common.h"

class Player final
{
public:
	Player();				// コンストラクタ.
	~Player();				// デストラクタ.

	void Update();			// 更新.
	void Draw();			// 描画.

	// グラフィックハンドルの取得.
	int GetGraphiclHandle(){ return graphicHandle; }

	// ポジションのgetter/setter.
	const VECTOR& GetPos() const { return pos; }
	void SetPos(const VECTOR set) { pos = set; }

	// ディレクションのgetter/setter.
	const VECTOR& GetDir() const { return dir; }
	void SetDir(const VECTOR set) { dir = set; }

private:
	int		graphicHandle;	// グラフィックハンドル.
	VECTOR	pos;			// ポジション.
	VECTOR	velocity;		// 移動力.
	VECTOR	dir;			// 回転方向.

	// 画像のサイズ。モデルでする人には不要.
	int	w;					// 幅.
	int	h;					// 高さ.

	const static float SPEED;	// 移動速度.

#if USE_MODEL
	const static float MODEL_SCALE;	// モデルのスケール.
#endif
};

#endif // _PLAYER_H_