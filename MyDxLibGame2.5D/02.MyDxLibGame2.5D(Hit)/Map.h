// 2023 Takeru Yui All Rights Reserved.
#pragma once

class WorldSprite;
class PlatinumLoader;

/// <summary>
/// 2Dの背景マップ
/// </summary>
class Map
{
public:
	Map();
	~Map();

	/// <summary>
	/// マップチップ構造体
	/// </summary>
	struct Chip
	{
		VECTOR			pos;		// 座標
		float			w, h;		// 幅、高さ
		int				col, row;	// 配置されている列、行

		int				chipKind;	// マップチップ種別
		WorldSprite*	sprite;
	};

	void Load(const TCHAR* fmfFilePath);
	void Update();
	void Draw();

	const Chip& GetChip(int col, int row) const;

	int GetMapColNum() const { return dataColNum; }
	int GetMapRowNum() const { return dataRowNum; }

	// マップチップのサイズ
	static const float	ChipSize;
	static const int	ChipPixelSize;

private:
	std::vector<std::vector<int>> currentData;
	int dataColNum = 0;
	int dataRowNum = 0;
	std::vector<Chip> chips;
	int chipGraph;
	PlatinumLoader* loader;
};

