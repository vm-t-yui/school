//-----------------------------------------------------------------------------
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#pragma once
#include "DxLib.h"

/// <summary>
/// プレイヤーの制御
/// </summary>
class Player final
{
public:
	Player();				// コンストラクタ
	~Player();				// デストラクタ

	void Update();			// 更新
	void Draw();			// 描画

	// ポジションのgetter
	const VECTOR& GetPos() const { return pos; }

private:
	int		modelHandle;	// モデルハンドル.
	VECTOR	pos;			// ポジション.
};
