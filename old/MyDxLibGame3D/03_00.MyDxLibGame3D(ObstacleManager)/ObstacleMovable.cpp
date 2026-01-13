// 2025 Takeru Yui All Rights Reserved.
#include "DxLib.h"
#include "ObstacleBase.h"
#include "ObstacleMovable.h"

#define _USE_MATH_DEFINES
#include <math.h>

/// <summary>
/// コンストラクタ
/// </summary>
ObstacleMovable::ObstacleMovable()
	: ObstacleBase("data/model/obstacleMobavle/obstacleMobavle.pmx")
	, rad(0)
{
	// 処理なし
}

/// <summary>
/// デストラクタ
/// </summary>
ObstacleMovable::~ObstacleMovable()
{
	// 処理なし
}

/// <summary>
/// コンストラクタ
/// </summary>
void ObstacleMovable::Update()
{
	rad += static_cast<float>((M_PI / 180) * 5);

	pos.y = 1.0f + sinf(rad);

	// ３Dモデルのポジション設定
	MV1SetPosition(modelHandle, pos);
}
