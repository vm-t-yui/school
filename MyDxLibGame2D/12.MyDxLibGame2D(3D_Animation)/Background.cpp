#include "DxLib.h"
#include "Background.h"
#include "GlobalConstants.h"

/// <summary>
/// 初期化
/// </summary>
void Background::Initialize()
{
	modelHandle = MV1LoadModel("data/model/Background/Cube_Bricks.mv1");
	// 3Dモデルが大きすぎるので、スケールで調整
	MV1SetScale(modelHandle, ModelScale);
}

/// <summary>
/// 描画
/// </summary>
void Background::Draw() const
{
	MV1SetPosition(modelHandle, Pos);

	// モデル描画
	MV1DrawModel(modelHandle);
}