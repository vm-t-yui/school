#include "DxLib.h"
#include "Screen.h"
#include "Map.h"

/// <summary>
/// マップデータ TODO: 外部データ化
/// </summary>
const int MapData[MapHeight][MapWidth] =
{
	0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
	1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1,
	1,0,0,1,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1,
	1,0,0,1,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,1,0,1,
	1,0,0,1,1, 1,1,0,0,0, 0,0,0,0,0, 0,0,1,0,1,

	1,0,0,0,0, 0,0,0,1,1, 0,0,0,0,0, 0,0,1,0,1,
	1,0,0,0,0, 0,0,0,0,0, 0,0,1,1,0, 0,0,1,0,1,
	1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,1,0,1,
	1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1,
	1,0,0,0,0, 0,0,1,1,0, 0,0,0,0,0, 1,0,0,0,1,

	1,0,0,0,0, 1,1,1,1,1, 0,0,0,0,1, 1,0,0,0,1,
	1,0,0,0,0, 1,1,1,1,1, 0,0,0,1,1, 1,0,0,0,1,
	1,0,0,0,0, 0,0,0,0,0, 0,0,0,1,1, 1,0,0,0,1,
	1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1,
	1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
};

/// <summary>
/// マップ情報初期化
/// </summary>
void InitMap(Map& map)
{
	// マップデータ読み込み
	for (int i = 0; i < MapHeight; i++)
	{
		for (int j = 0; j < MapWidth; j++)
		{
			map.mapChips[j][i].w = MapChipSize;
			map.mapChips[j][i].h = MapChipSize;
			map.mapChips[j][i].chipKind = MapData[j][i];
			map.mapChips[j][i].pos = VGet(j * map.mapChips[j][i].w, i * map.mapChips[j][i].h, 0);
		}
	}
}

/// <summary>
/// 指定ドットがマップチップの中にあるかどうか調べる
/// </summary>
bool IsInMapChip(Map& map, VECTOR dot)
{
	return false;
}

/// <summary>
/// マップ描画
/// </summary>
void DrawMap(Map& map)
{
	for (int i = 0; i < MapHeight; i++)
	{
		for (int j = 0; j < MapWidth; j++)
		{
			// １は当たり判定チップを表しているので１のところだけ描画
			if (map.mapChips[i][j].chipKind == 1)
			{
				DrawBox(static_cast<int>(map.mapChips[i][j].pos.x - map.mapChips[i][j].w * 0.5f),
					static_cast<int>(map.mapChips[i][j].pos.y - map.mapChips[i][j].h * 0.5f),
					static_cast<int>(map.mapChips[i][j].pos.x + map.mapChips[i][j].w * 0.5f),
					static_cast<int>(map.mapChips[i][j].pos.y + map.mapChips[i][j].h * 0.5f),
					GetColor(255, 255, 255), TRUE);
			}
		}
	}
}