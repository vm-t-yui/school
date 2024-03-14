// 2023 Takeru Yui All Rights Reserved.
#include<vector>
#include "Map.h"
#include "DxLib.h"
#include "WorldSprite.h"

const float Map::ChipSize = 0.725f;
const int Map::ChipPixelSize = 32;

/// <summary>
/// コンストラクタ
/// </summary>
Map::Map()
	: sprite(nullptr)
{
	// 処理なし
}

/// <summary>
/// デストラクタ
/// </summary>
Map::~Map()
{
	if (sprite != nullptr)
	{
		delete sprite;
	}
}

/// <summary>
/// ロード
/// </summary>
void Map::Load()
{
	// とりあえずマップロード
	int chipGraph = LoadGraph("data/map.png");

	// マップチップを表示するためのクラスWorldSpriteに
	// 元画像のハンドル、１チップあたりのサイズ、表示するマップチップ番号を渡す
	sprite = new WorldSprite();
	sprite->Initialize(chipGraph, ChipPixelSize, 65);

	// マップチップの位置と大きさを設定
	VECTOR chipHalfOffset = VGet(-Map::ChipSize * 0.5f, -Map::ChipSize * 0.5f, 0);					// マップチップの半分サイズ左下にずらすオフセット
	VECTOR chipPos = VAdd(VGet(0,0,0), chipHalfOffset);	// 真ん中ピボットなのでマップチップ半分サイズずらす+地面なので一つ下に
	sprite->SetTransform(chipPos, Map::ChipSize);
}

/// <summary>
/// 更新
/// </summary>
void Map::Update()
{
	// 処理なし
}

/// <summary>
/// 描画
/// </summary>
void Map::Draw()
{
	// ゆくゆくはカメラを持ってきて、カメラ範囲以外表示しないように
	sprite->Draw();
}


