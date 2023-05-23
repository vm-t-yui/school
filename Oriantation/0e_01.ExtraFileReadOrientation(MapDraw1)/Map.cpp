// 2023 Takeru Yui All Rights Reserved.
#include<vector>
#include "Map.h"
#include "DxLib.h"
#include "WorldSprite.h"

const int Map::Stage1Data[][StageDataColNum] =
{
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
};

/// <summary>
/// コンストラクタ
/// </summary>
Map::Map()
	: chipGraph(-1)
{
	currentData.clear();
	sprite = new WorldSprite();
}

/// <summary>
/// デストラクタ
/// </summary>
Map::~Map()
{
	if (sprite != nullptr)
	{
		delete sprite;
		sprite = nullptr;
	}
}

/// <summary>
/// ロード
/// </summary>
void Map::Load()
{
	// とりあえずマップロード
	chipGraph = LoadGraph("data/map.png");
	sprite->Initialize(chipGraph, 32, 65);
	sprite->SetTransform(VGet(-0.5f, -0.5f, 0), 1.0f);

	// 後々違うデータを動的に切り替えるため、currentDataにコピー
	const int(*targetDataRows)[StageDataColNum] = Stage1Data;

	currentData.clear();
	for (int i = 0; i < StageDataColNum; i++)
	{
		std::vector<int> newColData;
		for (int j = 0; j < StageDataColNum; j++)
		{
			newColData.push_back(targetDataRows[i][j]);
		}
		currentData.push_back(newColData);
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
	float size = 1.0f;
	sprite->Draw();
}


