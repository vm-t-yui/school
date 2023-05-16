#include<vector>
#include "Map.h"
#include "DxLib.h"

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
}

/// <summary>
/// デストラクタ
/// </summary>
Map::~Map()
{
	// 処理なし
}

/// <summary>
/// ロード
/// </summary>
void Map::Load()
{
	// とりあえずマップロード
	chipGraph = LoadGraph("data/mapChip1.png");

	//// 後々違うデータを動的に切り替えるため、currentDataにコピー
	//const int** targetData = (const int**)Stage1Data;

	//currentData.clear();
	//for (int i = 0; i < StageDataColNum; i++)
	//{
	//	std::vector<int> newColData;
	//	for (int j = 0; j < StageDataColNum; j++)
	//	{
	//		newColData.push_back(targetData[i][j]);
	//	}
	//	currentData.push_back(newColData);
	//}
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
	DrawBillboard3D(VGet(0,0,0), 0, 0, size, 0, chipGraph, FALSE);
	//DrawGraph3D(0, 0, 0, chipGraph, FALSE);
}


