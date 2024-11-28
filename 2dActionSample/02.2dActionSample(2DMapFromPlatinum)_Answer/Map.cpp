//-----------------------------------------------------------------------------
// 2024 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------

#include "DxLib.h"
#include "Screen.h"
#include "Map.h"

///// <summary>
///// マップデータ TODO: 外部データ化
///// </summary>
//const int MapData[MapHeight][MapWidth] =
//{
//	1,2,3,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,64,
//	16,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1,
//	32,17,18,1,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1,
//	48,0,0,1,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,1,0,1,
//	64,0,0,1,1, 1,1,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,1,0,1,
//
//	80,0,0,0,0, 0,0,0,1,1, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,1,0,1,
//	96,0,0,0,0, 0,0,0,0,0, 0,0,1,1,0, 0,0,1,1,0, 0,0,1,1,0, 0,0,1,0,1,
//	112,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,1,0,1,
//	113,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1,
//	114,0,0,0,0, 0,0,1,1,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 1,0,0,0,1,
//
//	115,0,0,0,0, 1,1,1,1,1, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1, 1,0,0,0,1,
//	116,0,0,0,0, 1,1,1,1,1, 0,0,0,0,0, 0,0,0,1,1, 0,0,0,1,1, 1,0,0,0,1,
//	117,0,0,0,0, 0,0,0,0,0, 0,0,0,1,1, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1,
//	118,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1,
//	119,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,96,
//};

/// <summary>
/// マップ情報初期化
/// </summary>
void InitMap(Map& map)
{
	// Platinumデータ読み込み
	map.loader.Load("data/stage1.fmf");

	// マップ画像読み込み
	map.graphHandle = LoadGraph("data/map.png");
	// マップデータ読み込み
	for (int wIndex = 0; wIndex < MapWidth; wIndex++)
	{
		for (int hIndex = 0; hIndex < MapHeight; hIndex++)
		{
			MapChip& mapChip = map.mapChips[wIndex][hIndex];
			mapChip.w = MapChipSize;
			mapChip.h = MapChipSize;

			// Platinumからのデータを入れる
			//mapChip.chipNo = ;
			mapChip.chipNo = map.loader.GetChipSpriteNo(LayerType::BackGround, wIndex, hIndex);

			// ポジションの基準を左上に
			mapChip.pos = VGet(wIndex * mapChip.w, hIndex * mapChip.h, 0);

			// 画像内での描画開始座標（左上）
			// MapGraphSize=512 MapChipSize=32 512*512の画像
			// 512 / 32 = 16なので、１行16個×16列の画像が並ぶ
			// 左上から順番に1ずつ番号をつけていく
			// 0   1  2  .... 15
			// 16 17 18  .... 31
			// .................
			// 240 ..........255
			// と番号が付くので、その番号×MapChipSize(32)が画像内の座標
			int chipSideNum = MapGraphSize / MapChipSize;  // 一片のマップチップ個数
			int inGraphXIdx = (mapChip.chipNo % chipSideNum); // 画像内の横インデックス
			int inGraphYIdx = (mapChip.chipNo / chipSideNum); // 画像内の縦インデックス
			mapChip.posInGraphX = inGraphXIdx * MapChipSize;
			mapChip.posInGraphY = inGraphYIdx * MapChipSize;
		}
	}
}

/// <summary>
/// マップ描画
/// </summary>
void DrawMap(const Map& map)
{
	for (int wIndex = 0; wIndex < MapWidth; wIndex++)
	{
		for (int hIndex = 0; hIndex < MapHeight; hIndex++)
		{
			// １は当たり判定チップを表しているので１のところだけ描画
			const MapChip& mapChip = map.mapChips[wIndex][hIndex];
			if (mapChip.chipNo > 0)
			{
				auto leftTopX = static_cast<int>(mapChip.pos.x);
				auto leftTopY = static_cast<int>(mapChip.pos.y);
				DrawRectGraph(leftTopX, leftTopY,
					mapChip.posInGraphX, mapChip.posInGraphY,
					MapChipSize, MapChipSize,
					map.graphHandle, true);
			}
		}
	}
}