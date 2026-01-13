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

	void Load(int stageNo);
	void Update();
	void Draw();

	// マップチップのサイズ
	static const float	ChipSize;
	static const int	ChipPixelSize;

private:
	static const int	StageData1ColNum = 16;		// マップ行
	static const int	StageData1RowNum = 20;		// マップ列
	static const int	Stage1Data[StageData1ColNum][StageData1RowNum];
	static const int	StageData2ColNum = 14;		// マップ行
	static const int	StageData2RowNum = 18;		// マップ列
	static const int	Stage2Data[StageData2ColNum][StageData2RowNum];
	
	std::vector<std::vector<int>> currentData;
	int dataColNum = 0;
	int dataRowNum = 0;
	std::vector<WorldSprite*> sprites;
	int chipGraph;
};

