// 2023 Takeru Yui All Rights Reserved.
#include<vector>
#include "Map.h"
#include "DxLib.h"
#include "WorldSprite.h"

const int Map::Stage1Data[StageData1ColNum][StageData1RowNum] =
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

const int Map::Stage2Data[StageData2ColNum][StageData2RowNum] =
{
	{65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65},
	{73,73, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,65},
	{73, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,65},
	{73, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,65},
	{73, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,65},
	{73, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,65},
	{73, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,65},
	{73, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,65},
	{73, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,65},
	{73, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,65},
	{73, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,65},
	{73, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,65},
	{73, 0,73, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,65},
	{73,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65},
};

const float Map::ChipSize = 0.725f;
const int Map::ChipPixelSize = 32;

/// <summary>
/// 配列へのポインタをもらってベクタを作成する
/// </summary>
std::vector<int> CreateArrayVector(const int targetDataCols[], int colNum)
{
	std::vector<int> newColData;
	for (int i = 0; i < colNum; i++)
	{
		newColData.push_back(targetDataCols[i]);
	}
	return newColData;
}

/// <summary>
/// コンストラクタ
/// </summary>
Map::Map()
	: chipGraph(-1)
{
	currentData.clear();
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
void Map::Load(int stageNo)
{
	// 外部からのint値入力で、Stage1Dataを使うかStage2Dataを使うか切り替える
	currentData.clear();
	switch (stageNo)
	{
	case 0:
		dataColNum = StageData1ColNum;
		dataRowNum = StageData1RowNum;
		for (int i = 0; i < StageData1ColNum; i++)
		{
			currentData.push_back(CreateArrayVector(Stage1Data[i], StageData1RowNum));
		}
		break;
	case 1:
		dataColNum = StageData2ColNum;
		dataRowNum = StageData2RowNum;
		for (int i = 0; i < StageData2ColNum; i++)
		{
			currentData.push_back(CreateArrayVector(Stage2Data[i], StageData2RowNum));
		}
		break;
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


