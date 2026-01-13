// 2023 Takeru Yui All Rights Reserved.
#include<vector>
#include "Map.h"
#include "DxLib.h"
#include "WorldSprite.h"

const int Map::Stage1Data[StageDataColNum][StageDataRowNum] =
{
	{65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65},
	{73,73, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,65},
	{73, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,65},
	{73, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,65},
	{73, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,65},
	{73, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,65},
	{73, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,65},
	{73, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,65},
	{73, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,65},
	{73, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,65},
	{73, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,65},
	{73, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,65},
	{73, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,65},
	{73, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,65},
	{73, 0,73, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,65},
	{73,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65},
};

const float Map::ChipSize = 0.725f;
const int Map::ChipPixelSize = 32;

/// <summary>
/// コンストラクタ
/// </summary>
Map::Map()
	: chipGraph(-1)
{
	// 処理なし
}

/// <summary>
/// デストラクタ
/// </summary>
Map::~Map()
{
	for (const auto& sprite : sprites)
	{
		if (sprite != nullptr)
		{
			delete sprite;
		}
	}
	sprites.clear();
}

/// <summary>
/// ロード
/// </summary>
void Map::Load()
{
	// とりあえずマップロード
	chipGraph = LoadGraph("data/map.png");

	// 「データ(Stage1Data)」を元に「実際に描画する人(WorldSprite)」に情報を渡していく
	// データと実際に描画する人を分離することで
	// データを外部ファイル化したり、描画時にちょっといじったりできる
	VECTOR chipLeftTopPos = VGet(0, StageDataColNum * ChipSize, 0);			// マップの描画開始位置（左上）
	for (int i = 0; i < StageDataColNum; i++)
	{
		for (int j = 0; j < StageDataRowNum; j++)
		{
			auto sprite = new WorldSprite();
			sprite->Initialize(chipGraph, ChipPixelSize, Stage1Data[i][j]);
			VECTOR chipHalfOffset = VGet(-Map::ChipSize * 0.5f, -Map::ChipSize * 0.5f, 0);					// マップチップの半分サイズ左下にずらすオフセット
			VECTOR chipPos = VAdd(VGet(j * Map::ChipSize,  (-i - 1) * Map::ChipSize, 0), chipHalfOffset);	// 真ん中ピボットなのでマップチップ半分サイズずらす+地面なので一つ下に
			chipPos = VAdd(chipPos, chipLeftTopPos);
			sprite->SetTransform(chipPos, Map::ChipSize);
			sprites.push_back(sprite);
		}
	}
}

/// <summary>
/// 更新
/// </summary>
void Map::Update()
{
	// 処理なし
}

/// <summary>
/// 描画
/// </summary>
void Map::Draw()
{
	// ゆくゆくはカメラを持ってきて、カメラ範囲以外表示しないように
	for (const auto& sprite : sprites)
	{
		sprite->Draw();
	}
}


