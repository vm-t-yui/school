#include "DxLib.h"
#include "Stage.h"

// ステージの初期化処理
void Stage::Initialize()
{
	// ステージモデルの読み込み
	ModelHandle = MV1LoadModel("ColTestStage.mqo");

	// モデル全体のコリジョン情報のセットアップ
	MV1SetupCollInfo(ModelHandle, -1);
}

// ステージの後始末処理
void Stage::Finalie()
{
	// ステージモデルの後始末
	MV1DeleteModel(ModelHandle);
}

// 描画
void Stage::Draw()
{
	MV1DrawModel(ModelHandle);
}