//-----------------------------------------------------------------------------
// 2024 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------

#include "DxLib.h"
#include "Screen.h"
#include "Map.h"
#include "Camera.h"

/// <summary>
/// マップデータ TODO: 外部データ化
/// </summary>
const int MapData[MapHeight][MapWidth] =
{
	0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
	1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1,
	1,0,0,1,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1,
	1,0,0,1,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,1,0,1,
	1,0,0,1,1, 1,1,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,1,0,1,

	1,0,0,0,0, 0,0,0,1,1, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,1,0,1,
	1,0,0,0,0, 0,0,0,0,0, 0,0,1,1,0, 0,0,1,1,0, 0,0,1,1,0, 0,0,1,0,1,
	1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,1,0,1,
	1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1,
	1,0,0,0,0, 0,0,1,1,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 1,0,0,0,1,

	1,0,0,0,0, 1,1,1,1,1, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1, 1,0,0,0,1,
	1,0,0,0,0, 1,1,1,1,1, 0,0,0,0,0, 0,0,0,1,1, 0,0,0,1,1, 1,0,0,0,1,
	1,0,0,0,0, 0,0,0,0,0, 0,0,0,1,1, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1,
	1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1,
	1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
};

/// <summary>
/// マップ情報初期化
/// </summary>
void InitMap(Map& map)
{
	// マップデータ読み込み
	for (int wIndex = 0; wIndex < MapWidth; wIndex++)
	{
		for (int hIndex = 0; hIndex < MapHeight; hIndex++)
		{
			MapChip& mapChip = map.mapChips[wIndex][hIndex];
			mapChip.w = MapChipSize;
			mapChip.h = MapChipSize;
			mapChip.chipKind = MapData[hIndex][wIndex];

			// ポジションの基準を真ん中に
			mapChip.pos = VGet(wIndex * mapChip.w + mapChip.w * 0.5f, hIndex * mapChip.h + mapChip.w * 0.5f, 0);
		}
	}
}

/// <summary>
/// マップ描画
/// </summary>
void DrawMap(const Map& map, const Camera& camera)
{
	for (int wIndex = 0; wIndex < MapWidth; wIndex++)
	{
		for (int hIndex = 0; hIndex < MapHeight; hIndex++)
		{
			// １は当たり判定チップを表しているので１のところだけ描画
			const MapChip& mapChip = map.mapChips[wIndex][hIndex];
			if (mapChip.chipKind == 1)
			{
				auto leftTop = static_cast<int>(mapChip.pos.x - mapChip.w * 0.5f);
				auto leftBottom = static_cast<int>(mapChip.pos.y - mapChip.h * 0.5f);
				auto rightTop = static_cast<int>(mapChip.pos.x + mapChip.w * 0.5f);
				auto rightBottom = static_cast<int>(mapChip.pos.y + mapChip.h * 0.5f);
				DrawBox(
					leftTop + static_cast<int>(camera.drawOffset.x),
					leftBottom + static_cast<int>(camera.drawOffset.y),
					rightTop + static_cast<int>(camera.drawOffset.x),
					rightBottom + static_cast<int>(camera.drawOffset.y),
					GetColor(255, 255, 255), TRUE);
			}
		}
	}
}