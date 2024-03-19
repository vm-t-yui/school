// 2023 Takeru Yui All Rights Reserved.
#include "DxLib.h"
#include <vector>
#include <cassert>
#include "Map.h"
#include "WorldSprite.h"
#include "PlatinumLoader.h"

/// <summary>
/// コンストラクタ
/// </summary>
Map::Map()
	: chipGraph(-1)
{
	currentData.clear();
	loader = new PlatinumLoader();
}

/// <summary>
/// デストラクタ
/// </summary>
Map::~Map()
{
	for (auto& chip : chips)
	{
		if (chip.sprite != nullptr)
		{
			delete chip.sprite;
			chip.sprite = nullptr;
		}
	}
	chips.clear();
	if (loader != nullptr)
	{
		delete loader;
		loader = nullptr;
	}
}

/// <summary>
/// ロード
/// </summary>
void Map::Load(const TCHAR* fmfFilePath)
{
	loader->Load(fmfFilePath);
	currentData.clear();

	std::vector<int> newColData;
	loader->GetMapSize(dataRowNum, dataColNum);		// マップのサイズ=データの行と列の長さを入れる
	for (int i = 0; i < dataColNum; i++)
	{
		newColData.clear();
		for (int j = 0; j < dataRowNum; j++)
		{
			int spriteNo = loader->GetChipSpriteNo(LayerType::BackGround, j, i);
			newColData.push_back(spriteNo);
		}
		currentData.push_back(newColData);
	}

	// とりあえずマップロード
	chipGraph = LoadGraph("data/map.png");

	// WorldSprite実体設定と位置初期化
	VECTOR chipLeftTopPos = VGet(0, dataColNum * ChipSize, 0);			// マップの描画開始位置（左上）
	for (int i = 0; i < dataColNum; i++)
	{
		for (int j = 0; j < dataRowNum; j++)
		{
			auto sprite = new WorldSprite();
			sprite->Initialize(chipGraph, ChipPixelSize, currentData[i][j]);
			VECTOR chipHalfOffset = VGet(-Map::ChipSize * 0.5f, -Map::ChipSize * 0.5f, 0);					// マップチップの半分サイズ左下にずらすオフセット
			VECTOR chipPos = VAdd(VGet(j * Map::ChipSize,  (-i - 1) * Map::ChipSize, 0), chipHalfOffset);	// 真ん中ピボットなのでマップチップ半分サイズずらす+地面なので一つ下に
			chipPos = VAdd(chipPos, chipLeftTopPos);
			sprite->SetTransform(chipPos, Map::ChipSize);
			
			// チップのサイズは真四角
			Chip chip;
			chip.pos = chipPos;
			chip.w = chip.w = Map::ChipSize;
			chip.col = i;
			chip.row = j;
			chip.sprite = sprite;
			chip.chipKind = currentData[i][j];
			chips.push_back(chip);
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
	for (const auto& chip : chips)
	{
		if (chip.chipKind > 0)
		{
			chip.sprite->Draw();
		}
	}
}

/// <summary>
/// 指定位置のマップチップを取得する
/// </summary>
const Map::Chip& Map::GetChip(int col, int row) const
{ 
	for (const auto& chip : chips)
	{
		if (col == chip.col && row == chip.row)
		{
			return chip;
		}
	}
	// ここまでくるという事はエラーなのでエラー吐いて0,0の物を返す
	assert(0 && "データ範囲指定エラー");
	return chips[0];
}

