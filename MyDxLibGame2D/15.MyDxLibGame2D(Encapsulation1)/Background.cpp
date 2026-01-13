#include <vector>
#include "DxLib.h"
#include "WorldSprite.h"
#include "Background.h"
#include "GlobalConstants.h"

namespace
{
	// モデルの表示スケールと位置補正
	// 適当な位置、適当なスケールで雑に背景のモデルを後ろの方に表示する
	const static VECTOR	ModelScale				= VGet(0.1f, 0.1f, 0.001f);
	const static VECTOR	Pos						= VGet(8.0f, -5.0f, 1.0f);

	const static int	MapChipPixel			= 32;		// マップチップのピクセル数
	const static float	MapChipOffsetZ			= 0.05f;	// マップチップ表示時のZオフセット
	const static float	TestSpriteOffsetZ		= 0.05f;	// テストスプライト表示時のZオフセット

	// マップチップの地面のスプライト番号
	const static int	MapChipGroundSpriteNo	= 1;
}

/// <summary>
/// コンストラクタ
/// </summary>
Background::Background()
{
	// 処理なし
}

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
	//spriteGraph = LoadGraph("data/texture/mapChip1.png");
	spriteGraph = LoadGraph("data/texture/map.png");

	// スクリーンサイズをMapChipサイズで割った分だけスプライト準備
	const int GroundMapChipNumW = Graphics::ScreenW / MapChipPixel;
	const int GroundMapChipNumH = Graphics::ScreenH / MapChipPixel;
	for (int i = 0; i < GroundMapChipNumW; i++)
	{
		WorldSprite worldSprite;

		// 初期化時に切り取って表示する画像のピクセルのサイズと、スプライト番号を渡す
		// LoadDivGraphと使い方は全く同じ！
		worldSprite.Initialize(spriteGraph, MapChipPixel, MapChipGroundSpriteNo);	// スプライト番号分ずらすことも

		// 3D表示基準のサイズなので、2D用のピクセルを3D用のスケールに変換
		const float mapChipSize3D = Graphics::ScaleFactor3D * MapChipPixel;

		// ポジションも2Dから3D用に修正（必要なら後でZ座標ずらすこともできる）
		VECTOR spritePos2D = VGet(
			(i * MapChipPixel) + (MapChipPixel * 0.5f),
			// 900/32で余り4なので余った4ドット足す
			((GroundMapChipNumH - 1) * MapChipPixel) + MapChipPixel * 0.5f + 4,
			0);
		VECTOR spritePos3D = Graphics::Get3DPosition(spritePos2D);

		// Zがゼロだとプレイヤーとかぶるのでちょっと後ろにずらす
		spritePos3D = VGet(spritePos3D.x, spritePos3D.y, spritePos3D.z + MapChipOffsetZ);

		worldSprite.SetTransform(spritePos3D, mapChipSize3D);

		worldSprites.emplace_back(worldSprite);
	}
	/////////////////////////////////

	// テスト用の画像
	auto testSpriteGraph	= LoadGraph("data/texture/saval.png");
	int testSpriteSize2DW, testSpriteSize2DH;
	GetGraphSize(testSpriteGraph, &testSpriteSize2DW, &testSpriteSize2DH);
	auto testSpritePos = Graphics::Get3DPosition(VGet(Graphics::ScreenW * 0.5f, Graphics::ScreenH * 0.5f, 0.0f));
	testSpritePos = VGet(testSpritePos.x, testSpritePos.y, testSpritePos.z + TestSpriteOffsetZ);
	const float testSpriteSize3DW = Graphics::ScaleFactor3D * testSpriteSize2DW;
	const float testSpriteSize3DH = Graphics::ScaleFactor3D * testSpriteSize2DH;
	testSingleSprite.Initialize(testSpriteGraph);
	testSingleSprite.SetTransform(testSpritePos, testSpriteSize3DW, testSpriteSize3DH);
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
	SetUseLighting(FALSE);	// ライト適用されたくないならSetUseLighting(FALSE)入れる
	for (const auto& worldSprite : worldSprites)
	{
		worldSprite.Draw();
	}
	testSingleSprite.Draw();
	SetUseLighting(TRUE);	// 戻すのを忘れずに
	/////////////////////////////////
}