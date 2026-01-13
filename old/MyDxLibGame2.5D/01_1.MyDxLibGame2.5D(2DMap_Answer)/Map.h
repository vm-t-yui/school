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
	// 元にする固定長二次元配列のマップ種類配置データ
	static const int Stage1Data[StageDataColNum][StageDataRowNum];
	
	// マップを実際に表示するためのWorldSpriteクラスたち
	//WorldSprite* sprites;			マップチップ１個
	//WorldSprite* sprites[10];		10個の配列
	//WorldSprite* sprites[StageDataColNum][StageDataRowNum];　StageDataColNum＊StageDataRowNum個の二次元配列
	std::vector<WorldSprite*> sprites;	// StageDataColNum＊StageDataRowNum個のvectorコンテナ（≒一次元配列）
	
	// マップチップの元画像ハンドル
	int chipGraph;
};

