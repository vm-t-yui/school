// 2023 Takeru Yui All Rights Reserved.
#pragma once

#include "Common.h"
#include "EnemyBase.h"

/// <summary>
/// 遅いエネミー
/// </summary>
class Enemy2 : public EnemyBase
{
public:
	Enemy2() {}
	virtual ~Enemy2() {}

	void Init();		// 初期化
	void Update();		// アップデート
};
