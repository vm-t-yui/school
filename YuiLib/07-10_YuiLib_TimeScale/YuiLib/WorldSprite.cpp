﻿#include "DxLib.h"
#include "WorldSprite.h"

using namespace YuiLib;

/// <summary>
/// 初期化
/// </summary>
/// <param name="textureGraph">テクスチャの画像ハンドル</param>
/// <param name="chipPixelSize">スプライトの１チップのピクセルサイズ</param>
/// <param name="spriteNo">スプライト番号</param>
void WorldSprite::Initialize(int textureGraph, int chipPixelSize, int spriteNo)
{
	this->textureGraph = textureGraph;
    this->chipPixelSize = chipPixelSize;

    // NOTE:初期化時に固定しているが、変更したければ自分で関数を追加する必要がある
    // ４頂点分のuvデータを設定
    GetGraphTextureSize(textureGraph, &texW, &texH);
    SetSpriteNo(spriteNo);

    // スペキュラ等は初期化時に固定
    for (int i = 0; i < 4; i++)
    {
        Vertex[i].dif = GetColorU8(255, 255, 255, 255);
        Vertex[i].spc = GetColorU8(0, 0, 0, 0);
        Vertex[i].norm = VGet(0.0f, 0.0f, -1.0f);   // 回転をサポートしないのでノーマルも固定

        // 補助テクスチャをサポートしないのでuv固定
        Vertex[i].su = 0.0f;
        Vertex[i].sv = 0.0f;
    }

    // ２ポリゴン分のインデックスデータをセット
    Index[0] = 0;
    Index[1] = 1;
    Index[2] = 2;
    Index[3] = 3;
    Index[4] = 2;
    Index[5] = 1;
}

/// <summary>
/// スプライト番号を変更する
/// </summary>
/// <param name="spriteNo">スプライト番号</param>
void WorldSprite::SetSpriteNo(int spriteNo)
{
    int chipNum = texW / chipPixelSize;
    int chipNoX = spriteNo % chipNum;
    int chipNoY = spriteNo / chipNum;
    float oneChipUVRate = 1.0f / (float)chipNum;   // テクスチャ全部を1.0とした時のcihp一個に対するuvのサイズ
    Vertex[0].u = (chipNoX + 0.0f) * oneChipUVRate;
    Vertex[0].v = (chipNoY + 0.0f) * oneChipUVRate;
    Vertex[1].u = (chipNoX + 1.0f) * oneChipUVRate;
    Vertex[1].v = (chipNoY + 0.0f) * oneChipUVRate;
    Vertex[2].u = (chipNoX + 0.0f) * oneChipUVRate;
    Vertex[2].v = (chipNoY + 1.0f) * oneChipUVRate;
    Vertex[3].u = (chipNoX + 1.0f) * oneChipUVRate;
    Vertex[3].v = (chipNoY + 1.0f) * oneChipUVRate;
}

/// <summary>
/// サイズとポジションに応じて４頂点分の頂点位置を調整
/// </summary>
/// <param name="pos">ポジション</param>
/// <param name="chipSize">配置するワールドスプライトのサイズ</param>
void WorldSprite::SetTransform(const VECTOR& pos, float spriteSize)
{
    this->pos = pos;
    // ピボット中心で設定
    Vertex[0].pos = VScale(VGet(-1.0f, 1.0f, 0.0f), spriteSize * 0.5f);
    Vertex[1].pos = VScale(VGet(1.0f, 1.0f, 0.0f), spriteSize * 0.5f);
    Vertex[2].pos = VScale(VGet(-1.0, -1.0f, 0.0f), spriteSize * 0.5f);
    Vertex[3].pos = VScale(VGet(1.0f, -1.0f, 0.0f), spriteSize * 0.5f);
    for (int i = 0; i < 4; i++)
    {
        Vertex[i].pos = VAdd(Vertex[i].pos, pos);
    }
}

/// <summary>
/// カラー設定
/// </summary>
void WorldSprite::SetColor(int color, float alpha)
{
    for (int i = 0; i < 4; i++)
    {
        int r = (color & (0xff0000)) >> 16;
        int g = (color & (0x00ff00)) >> 8;
        int b = (color & (0x0000ff)) >> 0;
        int intAlpha = static_cast<int>(255.0f * alpha);
        Vertex[i].dif = GetColorU8(r, g, b, intAlpha);
    }
}

/// <summary>
/// 描画
/// </summary>
void WorldSprite::Draw()
{
    // ２ポリゴンの描画
    DrawPolygonIndexed3D(Vertex, 4, Index, 2, textureGraph, TRUE);
}