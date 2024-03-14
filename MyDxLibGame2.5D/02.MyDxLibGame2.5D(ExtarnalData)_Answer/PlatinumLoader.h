#pragma once

enum class LayerType {
	BackGround,//背景
	Enemies,//敵
	Event,//イベント
};

//マップの広さ * レイヤー数なので、2つのベクターを入れ子にしてる
using MapData_t = std::vector < std::vector<unsigned char>>;


/// <summary>
/// Platinumのデータをロードする
/// </summary>
class PlatinumLoader
{
public:
	/// <summary>
	/// fmfファイルを読み込んでmapData_に中身を入れる
	/// </summary>
	/// <param name="filePath">fmfファイルのファイルパス</param>
	void Load(const TCHAR* filePath);
	
	/// <summary>
	/// mapData_の中身を直接取得する
	/// </summary>
	const MapData_t& GetMapData()const;
	
	/// <summary>
	/// Loadで読み込んだMapData_tの中身（マップチップのデータ）を個別に取得する
	/// </summary>
	/// <param name="layerType">取得するマップチップのレイヤーID</param>
	/// <param name="chipX">取得するマップチップのX座標</param>
	/// <param name="chipY">取得するマップチップのY座標</param>
	/// <returns>引数で指定した場所のマップチップのスプライト番号</returns>
	const int GetChipSpriteNo(LayerType layerType,int chipX,int chipY)const;
	
	/// <summary>
	/// 
	/// </summary>
	/// <param name="width"></param>
	/// <param name="height"></param>
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

