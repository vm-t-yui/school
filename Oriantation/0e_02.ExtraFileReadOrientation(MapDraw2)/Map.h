// 2023 Takeru Yui All Rights Reserved.
#pragma once
#include <vector>

class WorldSprite;

/// <summary>
/// 2Dの背景マップ
/// </summary>
class Map
{
public:
	Map();
	~Map();

	void Load();
	void Update();
	void Draw();

	// マップチップのサイズ
	static const float	ChipSize;
	static const int	ChipPixelSize;
	static const int	StageDataColNum = 16;		// マップ行
	static const int	StageDataRowNum = 20;		// マップ列

private:
	static const int Stage1Data[StageDataColNum][StageDataRowNum];
	
	std::vector<std::vector<int>> currentData;
	std::vector<WorldSprite*> sprites;
	int chipGraph;
};

