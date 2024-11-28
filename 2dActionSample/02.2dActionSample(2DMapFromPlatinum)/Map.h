//-----------------------------------------------------------------------------
// 2024 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------

#pragma once

//-----------------------------------------//
// 定数
//-----------------------------------------//
const int MapChipSize = 32;                     // 一つのチップのサイズ
const int MapWidth = 640 / MapChipSize + (10);  // マップの横幅
const int MapHeight = 480 / MapChipSize;		// マップの縦幅
const int MapGraphSize = 512;					// マップ画像のサイズ（正方形）

/// <summary>
/// マップチップ構造体
/// </summary>
struct MapChip
{
	int chipNo;
	float w, h;			// 幅、高さ
	VECTOR pos;			// 座標
	int posInGraphX;	// 画像内のX座標
	int posInGraphY;	// 画像内のY座標
};

/// <summary>
/// マップ構造体
/// </summary>
struct Map
{
	MapChip mapChips[MapWidth][MapHeight];
	int graphHandle;
};

//-----------------------------------------//
// プロトタイプ宣言
//-----------------------------------------//
void InitMap(Map& map);
void DrawMap(const Map& map);