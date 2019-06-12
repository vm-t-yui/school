//-----------------------------------------------------------------------------
// @brief  障害物：動かない.
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#ifndef _OBSTRUCT_STATIC_H_
#define _OBSTRUCT_STATIC_H_

#include "DxLib.h"

class ObstructStatic
{
public:
	ObstructStatic();				// コンストラクタ.
	virtual ~ObstructStatic();		// デストラクタ.

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

};

#endif // _OBSTACLE_BASE_H_

