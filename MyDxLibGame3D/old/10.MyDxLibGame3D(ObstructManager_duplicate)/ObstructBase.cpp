﻿//-----------------------------------------------------------------------------
// @brief  障害物基底クラス.
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#include "DxLib.h"
#include "ObstructBase.h"

#define USE_MODEL_DUPLICATE 1

//-----------------------------------------------------------------------------
// @brief  コンストラクタ.
//-----------------------------------------------------------------------------
ObstructBase::ObstructBase(int sourceModelHandle)
	:	modelHandle		(-1)
	 ,	pos				(VGet(0, 0, 0))
{
	// ３Ｄモデルの読み込み
#if USE_MODEL_DUPLICATE
	modelHandle = MV1DuplicateModel(sourceModelHandle);
	if (modelHandle < 0)
	{
		printfDx("データ読み込みに失敗. sourceId:%d", sourceModelHandle);
	}
#else
	modelHandle = MV1LoadModel("data/model/obstructStatic/obstructStatic.pmd");
	if (modelHandle < 0)
	{
		printfDx("ObstructStatic:データ読み込みに失敗");
	}
#endif
}

//-----------------------------------------------------------------------------
// @brief  デストラクタ.
//-----------------------------------------------------------------------------
ObstructBase::~ObstructBase()
{
	// モデルのアンロード.
	MV1DeleteModel(modelHandle);
}
//-----------------------------------------------------------------------------
// @brief  描画.
//-----------------------------------------------------------------------------
void ObstructBase::Draw()
{
	// ３Ｄモデルの描画
	MV1DrawModel(modelHandle);
}