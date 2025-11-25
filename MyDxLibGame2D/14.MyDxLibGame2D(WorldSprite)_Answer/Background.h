#pragma once

class WorldSprite;

/// <summary>
/// 背景クラス
/// </summary>
class Background
{
public:
	int							modelHandle;
	int							spriteGraph;
	std::vector<WorldSprite>	worldSprites;
	WorldSprite					testSingleSprite;

	// モデルの表示スケールと位置補正
	// 適当な位置、適当なスケールで雑に背景のモデルを後ろの方に表示する
	inline const static VECTOR	ModelScale	= VGet(0.1f, 0.1f, 0.001f);
	inline const static VECTOR	Pos			= VGet(8.0f, -5.0f, 1.0f);

	constexpr static int	MapChipPixel		= 32;		// マップチップのピクセル数
	constexpr static float	MapChipOffsetZ		= 0.05f;	// マップチップ表示時のZオフセット
	constexpr static float	TestSpriteOffsetZ	= 0.05f;	// テストスプライト表示時のZオフセット

	// マップチップの地面のスプライト番号
	constexpr static int	MapChipGroundSpriteNo = 1;

	void Initialize();
	void Draw() const;
};