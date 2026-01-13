//-----------------------------------------------------------------------------
// @brief  障害物：浮遊する.
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#include "DxLib.h"
#include "ObstructFloat.h"

#define _USE_MATH_DEFINES
#include <math.h>

/// <summary>
/// コンストラクタ
/// </summary>
ObstructFloat::ObstructFloat(int sourceModelHandle)
	:	ObstructBase	(sourceModelHandle)
	 ,	rad				(0)
{
	// 処理なし
}

/// <summary>
/// デストラクタ
/// </summary>
ObstructFloat::~ObstructFloat()
{
	// 処理なし
}

/// <summary>
/// 更新
/// </summary>
void ObstructFloat::Update()
{
	rad += static_cast<float>((M_PI / 180) * 5);

	pos.y = 1.0f + sinf(rad);

	// ３Dモデルのポジション設定
	MV1SetPosition(modelHandle, pos);
}
