#pragma once
#include <DxLib.h>
#include <vector>

enum class LayerType {
	BackGround,//背景
	Enemies,//敵
	Event,//イベント
};

//マップの広さ * レイヤー数なので、2つのベクターを入れ子にしてる
using MapData_t = std::vector < std::vector<unsigned char>>;

//ステージ情報を管理するクラス
class Stage
{
public:
	void Load(const TCHAR* filePath);
	const MapData_t& GetMapData()const;
	const int GetChipId(int layerId,int chipX,int chipY)const;
	void GetMapSize(int& width,int& height);
private:
	/// <summary>
	/// データの内容をXY転置する
	/// </summary>
	/// <param name="layerId">レイヤーID</param>
	void TransposeMapData(int layerId);
private:

	MapData_t mapData_;
	int mapWidth_ = 0;
	int mapHeight_ = 0;
};

