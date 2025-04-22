// 2025 Takeru Yui All Rights Reserved.
#include "DxLib.h"
#include "ObstacleBase.h"
#include "ObstacleKinematic.h"

/// <summary>
/// コンストラクタ
/// </summary>
ObstacleKinematic::ObstacleKinematic()
	:  ObstacleBase("data/model/obstacleKinematic/obstacleKinematic.pmd")
{
	// 処理なし
}

/// <summary>
/// デストラクタ
/// </summary>
ObstacleKinematic::~ObstacleKinematic()
{
	// 処理なし
}

/// <summary>
/// 更新
/// </summary>
void ObstacleKinematic::Update()
{
	// ３Dモデルのポジション設定
	MV1SetPosition(modelHandle, pos);
}
