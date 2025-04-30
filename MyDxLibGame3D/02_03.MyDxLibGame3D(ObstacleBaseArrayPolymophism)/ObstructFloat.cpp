﻿//-----------------------------------------------------------------------------
// @brief  障害物：浮遊する.
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#include "ObstructFloat.h"

#define _USE_MATH_DEFINES
#include <math.h>

//-----------------------------------------------------------------------------
// @brief  コンストラクタ.
//-----------------------------------------------------------------------------
ObstructFloat::ObstructFloat()
{
	// ３Ｄモデルの読み込み
	modelHandle = MV1LoadModel("data/model/obstructFloat/obstructFloat.pmx");
	if (modelHandle < 0)
	{
		printfDx("ObstructFloat:データ読み込みに失敗");
	}

	pos = VGet(0, 0, 0);
	rad = 0;
}

//-----------------------------------------------------------------------------
// @brief  デストラクタ.
//-----------------------------------------------------------------------------
ObstructFloat::~ObstructFloat()
{
	// モデルのアンロード.
	MV1DeleteModel(modelHandle);
}

//-----------------------------------------------------------------------------
// @brief  更新.
//-----------------------------------------------------------------------------
void ObstructFloat::Update()
{
	rad += static_cast<float>((M_PI / 180) * 5);

	pos.y = 1.0f + sinf(rad);

	// ３Dモデルのポジション設定
	MV1SetPosition(modelHandle, pos);
}
