#include "DxLib.h"
#include "WorldSprite.h"
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

	/////////////////////////////////
	// worldSpriteに表示する為のテクスチャ
	spriteGraph = LoadGraph("data/texture/mapChip1.png");

	// 初期化時に切り取って表示する画像のピクセルのサイズと、スプライト番号を渡す
	// LoadDivGraphと使い方は全く同じ！
	const int mapChipPixel = 32;
	worldSprite.Initialize(spriteGraph, mapChipPixel, 0);

	// 3D表示基準のサイズなので、2D用のピクセルを3D用のスケールに変換
	const float mapChipSize3D = Graphics::ScaleFactor3D * mapChipPixel;

	// ポジションも2Dから用に修正（必要なら後でZ座標ずらすこともできる）
	VECTOR spritePos2D = VGet(mapChipPixel * 0.5f, mapChipPixel * 0.5f, 0);
	VECTOR spritePos3D = Graphics::Get3DPosition(spritePos2D);

	worldSprite.SetTransform(spritePos3D, mapChipSize3D);
	/////////////////////////////////
}

/// <summary>
/// 描画
/// </summary>
void Background::Draw() const
{
	MV1SetPosition(modelHandle, Pos);

	// モデル描画
	MV1DrawModel(modelHandle);

	/////////////////////////////////
	// WorldSprite描画
	worldSprite.Draw();
	/////////////////////////////////
}