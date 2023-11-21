#include "DxLib.h"
#include "Stage.h"

/// <summary>
/// 初期化
/// </summary>
void Stage::Initialize()
{
	// ステージモデルの読み込み
	ModelHandle = MV1LoadModel("ColTestStage.mqo");

	// モデル全体のコリジョン情報のセットアップ
	MV1SetupCollInfo(ModelHandle, -1);
}

/// <summary>
/// 終了
/// </summary>
void Stage::Finalie()
{
	// ステージモデルの後始末
	MV1DeleteModel(ModelHandle);
}

/// <summary>
/// 描画
/// </summary>
void Stage::Draw()
{
	MV1DrawModel(ModelHandle);
}