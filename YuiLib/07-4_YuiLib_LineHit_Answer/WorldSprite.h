// 2023 Takeru Yui All Rights Reserved.
#pragma once

/// <summary>
/// 3D座標空間にスプライトを表示する仕組み（ピボットは中心固定、画像のgraphは外で管理）
/// </summary>
class WorldSprite
{
public:
	void Initialize(int textureGraph, int chipPixelSize, int spriteNo);
	void SetTransform(const VECTOR& pos, float spriteSize);
	void Draw();

	const VECTOR& GetPos()const { return pos; }
	
private:
	int			textureGraph;	// スプライトを作成する元テクスチャのグラフィックハンドル
	VECTOR		pos;			// 描画ポジション
	VERTEX3D	Vertex[4];		// 頂点バッファ
	WORD		Index[6];		// インデックスバッファ
};

