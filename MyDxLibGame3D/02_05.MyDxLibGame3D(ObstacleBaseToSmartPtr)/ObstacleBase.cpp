// 2025 Takeru Yui All Rights Reserved.
#include "DxLib.h"
#include "ObstacleBase.h"

/// <summary>
/// コンストラクタ
/// </summary>
ObstacleBase::ObstacleBase(const char* path)
	:  modelHandle(-1)
	, pos(VGet(0, 0, 0))
{
	// ３Ｄモデルの読み込み
	modelHandle = MV1LoadModel(path);
	if (modelHandle < 0)
	{
		printfDx("ObstacleBase:データ読み込みに失敗");
	}
}

/// <summary>
/// デストラクタ
/// </summary>
ObstacleBase::~ObstacleBase()
{
	// モデルのアンロード.
	MV1DeleteModel(modelHandle);
}

/// <summary>
/// ３Ｄモデルの描画
/// </summary>
void ObstacleBase::Draw() const
{
    MV1DrawModel(modelHandle);
}
