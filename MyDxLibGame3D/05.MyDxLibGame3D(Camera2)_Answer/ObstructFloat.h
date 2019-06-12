//-----------------------------------------------------------------------------
// @brief  障害物：浮遊する.
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#ifndef _OBSTRUCT_FLOAT_H_
#define _OBSTRUCT_FLOAT_H_

#include "DxLib.h"

class ObstructFloat
{
public:
	ObstructFloat();				// コンストラクタ.
	virtual ~ObstructFloat();		// デストラクタ.

    void Update();					// 更新.
    void Draw();					// 描画.

	// モデルハンドルの取得.
    int GetModelHandle() { return modelHandle; }

    // ポジションのgetter/setter.
    const VECTOR& GetPos() const { return pos; }
    void SetPos(const VECTOR set) { pos = set; }

private:
    int		modelHandle;	// モデルハンドル.
    VECTOR	pos;			// ポジション.

	float rad;						// sin移動用のラジアン関数.
};

#endif // _OBSTRUCT_FLOAT_H_

