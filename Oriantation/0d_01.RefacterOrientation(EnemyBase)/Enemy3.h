#pragma once

#include "Common.h"
#include "EnemyBase.h"

/// <summary>
/// 大きくて固いエネミー
/// </summary>
class Enemy3 : public EnemyBase
{
public:
	void Init();		// 初期化
	void Update();		// アップデート
};
