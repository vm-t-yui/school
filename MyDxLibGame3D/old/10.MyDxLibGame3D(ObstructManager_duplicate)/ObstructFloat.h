//-----------------------------------------------------------------------------
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#pragma once
#include "ObstructBase.h"

/// <summary>
/// 障害物：浮遊
/// </summary>
class ObstructFloat : public ObstructBase
{
public:
	ObstructFloat(int sourceModelHandle);	// コンストラクタ
	virtual ~ObstructFloat();				// デストラクタ

	void Update() override;					// 更新

private:
	float rad;								// sin移動用のラジアン関数
};