//-----------------------------------------------------------------------------
// @brief  プレイヤークラス.
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "DxLib.h"

class Player
{
public:
	Player();				// コンストラクタ.
	~Player();				// デストラクタ.

	void Update();			// 制御.
	void Draw();			// 描画.
};

#endif // _PLAYER_H_