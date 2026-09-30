// 2026 Takeru Yui All rights reserved.
#include <cmath>
#include "WorldSprite.h"
#include "SystemConfig.h"

namespace System
{
    /// <summary>
    /// WorldSpriteを初期状態で作成する
    /// </summary>
    WorldSprite::WorldSprite()
    {
        indices[0] = 0;
        indices[1] = 1;
        indices[2] = 2;
        indices[3] = 3;
        indices[4] = 2;
        indices[5] = 1;

        UpdateVertices();
    }

    /// <summary>
    /// Quadに表示するテクスチャを設定する
    /// </summary>
    void WorldSprite::SetTexture(int graphHandle)
    {
        textureHandle = graphHandle;
    }

    /// <summary>
    /// Pivotが置かれる3D位置を設定する
    /// </summary>
    void WorldSprite::SetPosition(const VECTOR& newPosition)
    {
        position = newPosition;
        UpdateVertices();
    }

    /// <summary>
    /// Quadのワールド空間上の大きさを設定する
    /// </summary>
    void WorldSprite::SetSize(float newWidth, float newHeight)
    {
#if SYSTEM_ENABLE_DEBUG_CHECK
        if (newWidth <= 0.0f || newHeight <= 0.0f)
        {
            printfDx("[System::WorldSprite] Unusual size: %.2f x %.2f\n", newWidth, newHeight);
        }
#endif
        width  = newWidth;
        height = newHeight;
        UpdateVertices();
    }

    /// <summary>
    /// 画像内のPivotを0.0から1.0の範囲で設定する
    /// </summary>
    void WorldSprite::SetPivot(float x, float y)
    {
#if SYSTEM_ENABLE_DEBUG_CHECK
        if (x < 0.0f || x > 1.0f || y < 0.0f || y > 1.0f)
        {
            printfDx("[System::WorldSprite] Pivot is outside 0.0 - 1.0: %.2f, %.2f\n", x, y);
        }
#endif
        pivotX = x;
        pivotY = y;
        UpdateVertices();
    }

    /// <summary>
    /// QuadをPivot中心に度数法で面内回転する
    /// </summary>
    void WorldSprite::SetRotation(float degree)
    {
        rotationDegree = degree;
        UpdateVertices();
    }

    /// <summary>
    /// 使用するUV範囲を設定する
    /// </summary>
    void WorldSprite::SetUv(float left, float top, float right, float bottom)
    {
        uvLeft   = left;
        uvTop    = top;
        uvRight  = right;
        uvBottom = bottom;
        UpdateVertices();
    }

    /// <summary>
    /// Quadへ掛け合わせる色を0xRRGGBB形式で設定する
    /// </summary>
    void WorldSprite::SetColor(unsigned int newColor)
    {
        color = newColor;
        UpdateVertices();
    }

    /// <summary>
    /// Quadを描画する
    /// </summary>
    void WorldSprite::Draw() const
    {
        if (textureHandle < 0)
        {
#if SYSTEM_ENABLE_DEBUG_CHECK
            printfDx("[System::WorldSprite] Invalid texture handle: %d\n", textureHandle);
#endif
            return;
        }

        DrawPolygonIndexed3D(vertices, 4, indices, 2, textureHandle, TRUE);
    }

    /// <summary>
    /// 現在の設定からQuadの頂点を作り直す
    /// </summary>
    void WorldSprite::UpdateVertices()
    {
        // Pivotは画像と同じ感覚で、(0, 0)を左上、(1, 1)を右下として扱う
        const float left   = -width * pivotX;
        const float right  = width * (1.0f - pivotX);
        const float top    = height * pivotY;
        const float bottom = -height * (1.0f - pivotY);

        VECTOR localPositions[4]
        {
            VGet(left, top, 0.0f),
            VGet(right, top, 0.0f),
            VGet(left, bottom, 0.0f),
            VGet(right, bottom, 0.0f)
        };

        const float rotationRadian = rotationDegree * (DX_PI_F / 180.0f);
        const float cosine         = std::cos(rotationRadian);
        const float sine           = std::sin(rotationRadian);

        for (int i = 0; i < 4; i++)
        {
            const float rotatedX = localPositions[i].x * cosine - localPositions[i].y * sine;
            const float rotatedY = localPositions[i].x * sine + localPositions[i].y * cosine;

            vertices[i].pos  = VAdd(position, VGet(rotatedX, rotatedY, 0.0f));
            vertices[i].norm = VGet(0.0f, 0.0f, -1.0f);
            vertices[i].dif  = GetColorU8(
                (color >> 16) & 0xff,
                (color >> 8) & 0xff,
                color & 0xff,
                255);
            vertices[i].spc  = GetColorU8(0, 0, 0, 0);
            vertices[i].su   = 0.0f;
            vertices[i].sv   = 0.0f;
        }

        vertices[0].u = uvLeft;
        vertices[0].v = uvTop;
        vertices[1].u = uvRight;
        vertices[1].v = uvTop;
        vertices[2].u = uvLeft;
        vertices[2].v = uvBottom;
        vertices[3].u = uvRight;
        vertices[3].v = uvBottom;
    }
}
