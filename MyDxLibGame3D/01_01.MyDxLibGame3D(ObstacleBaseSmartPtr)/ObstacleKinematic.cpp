// 2025 Takeru Yui All Rights Reserved.
#include "DxLib.h"
#include "ObstacleKinematic.h"

/// <summary>
/// コンストラクタ
/// </summary>
ObstacleKinematic::ObstacleKinematic()
{
	// ３Ｄモデルの読み込み
	modelHandle = MV1LoadModel("data/model/obstacleKinematic/obstacleKinematic.pmd");
	if (modelHandle < 0)
	{
		printfDx("ObstacleKinematic:データ読み込みに失敗");
	}

	pos = VGet(0, 0, 0);
}

/// <summary>
/// デストラクタ
/// </summary>
ObstacleKinematic::~ObstacleKinematic()
{
	// モデルのアンロード.
	MV1DeleteModel(modelHandle);
}

/// <summary>
/// 更新
/// </summary>
void ObstacleKinematic::Update()
{
	// ３Dモデルのポジション設定
	MV1SetPosition(modelHandle, pos);
}

/// <summary>
/// ３Ｄモデルの描画
/// </summary>
void ObstacleKinematic::Draw() const
{
	MV1DrawModel(modelHandle);
}

