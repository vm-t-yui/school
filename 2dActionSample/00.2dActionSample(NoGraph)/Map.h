//-----------------------------------------------------------------------------
// 2024 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------

#pragma once

//-----------------------------------------//
// 定数
//-----------------------------------------//
const int MapChipSize = 32;                           // 一つのチップのサイズ
const int MapWidth = ScreenWidth / MapChipSize;    // マップの横幅
const int MapHeight = ScreenHeight / MapChipSize;   // マップの縦幅

/// <summary>
/// マップチップ構造体
/// </summary>
struct MapChip
{
	VECTOR pos;	// 座標
	float w, h;	// 幅、高さ
	int chipKind;
};

/// <summary>
/// マップ構造体
/// </summary>
struct Map
{
	MapChip mapChips[MapWidth][MapHeight];
};

//-----------------------------------------//
// プロトタイプ宣言
//-----------------------------------------//
void InitMap(Map& map);
void DrawMap(const Map& map);