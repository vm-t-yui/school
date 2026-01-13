// 2025 Takeru Yui All Rights Reserved.
#include "DxLib.h"
#include "ObstacleMovable.h"

#define _USE_MATH_DEFINES
#include <math.h>

/// <summary>
/// コンストラクタ
/// </summary>
ObstacleMovable::ObstacleMovable()
{
	// ３Ｄモデルの読み込み
	modelHandle = MV1LoadModel("data/model/obstacleMovable/obstacleMovable.pmx");
	if (modelHandle < 0)
	{
		printfDx("ObstacleMovable:データ読み込みに失敗");
	}

	pos = VGet(0, 0, 0);
	rad = 0;
}

/// <summary>
/// デストラクタ
/// </summary>
ObstacleMovable::~ObstacleMovable()
{
	// モデルのアンロード.
	MV1DeleteModel(modelHandle);
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

/// <summary>
/// ３Ｄモデルの描画
/// </summary>
void ObstacleMovable::Draw() const
{
	MV1DrawModel(modelHandle);
}

