// 2026 Takeru Yui All rights reserved.
#pragma once

#include "DxLib.h"

// ============================================================================
// WorldSprite
//
// 3D空間にテクスチャ付きQuadを描画するための実装例
// 背景専用ではなく、看板・板ポリゴン・簡単なエフェクトなどにも利用できる
// 回転はQuad自身の面内回転だけを扱い、必要なら自由に拡張してよい
// ============================================================================
namespace System
{
    class WorldSprite
    {
    public:
        /// <summary>
        /// WorldSpriteを初期状態で作成する
        /// </summary>
        WorldSprite();

        /// <summary>
        /// Quadに表示するテクスチャを設定する
        /// </summary>
        void SetTexture(int graphHandle);

        /// <summary>
        /// Pivotが置かれる3D位置を設定する
        /// </summary>
        void SetPosition(const VECTOR& position);

        /// <summary>
        /// Quadのワールド空間上の大きさを設定する
        /// </summary>
        void SetSize(float width, float height);

        /// <summary>
        /// 画像内のPivotを0.0から1.0の範囲で設定する
        /// </summary>
        void SetPivot(float x, float y);

        /// <summary>
        /// QuadをPivot中心に度数法で面内回転する
        /// </summary>
        void SetRotation(float degree);

        /// <summary>
        /// 使用するUV範囲を設定する
        /// </summary>
        void SetUv(float left, float top, float right, float bottom);

        /// <summary>
        /// Quadへ掛け合わせる色を0xRRGGBB形式で設定する
        /// </summary>
        void SetColor(unsigned int color);

        /// <summary>
        /// Quadを描画する
        /// </summary>
        void Draw() const;

    private:
        int          textureHandle  = -1;
        VECTOR       position       = VGet(0.0f, 0.0f, 0.0f);
        float        width          = 1.0f;
        float        height         = 1.0f;
        float        pivotX         = 0.5f;
        float        pivotY         = 0.5f;
        float        rotationDegree = 0.0f;
        float        uvLeft         = 0.0f;
        float        uvTop          = 0.0f;
        float        uvRight        = 1.0f;
        float        uvBottom       = 1.0f;
        unsigned int color          = 0xffffff;
        VERTEX3D     vertices[4]{};
        WORD         indices[6]{};

        /// <summary>
        /// 現在の設定からQuadの頂点を作り直す
        /// </summary>
        void UpdateVertices();
    };
}
