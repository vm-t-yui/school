// 2026 Takeru Yui All rights reserved.
#include <cmath>
#include <unordered_map>
#include "DxLib.h"
#include "Beginner2D3D.h"
#include "../System/Model.h"
#include "../System/Camera.h"
#include "../System/WorldSprite.h"

namespace
{
    // 疑似2D表示のために、この中間層だけが使用する設定
    static const float WorldScale                = 0.01f;
    static const float Pseudo2DFieldOfViewDegree = 10.0f;
    static const float CameraNearDistance        = 1.0f;
    static const float CameraFarDistance         = 200.0f;
    static const float ModelDepth                = 0.0f;
    static const float BackgroundDepth           = 0.01f;

#if BEGINNER_ENABLE_MODEL_SIZE_DEBUG
    static const float ModelSizeDebugDepth          = -0.01f;
    static const float ModelSizeDebugCenterHalfSize = 6.0f;
#endif

    class Model2DSettings
    {
    public:
        float width   = 0.0f;
        float height  = 0.0f;
        float offsetX = 0.0f;
        float offsetY = 0.0f;
    };

    System::Camera camera;
    std::unordered_map<int, Model2DSettings> modelSettings;
    std::unordered_map<int, System::WorldSprite> worldSprites;

    /// <summary>
    /// 画面座標を疑似2D表示用の3D座標へ変換する
    /// </summary>
    VECTOR ConvertScreenToWorldPosition(float x, float y, float depth)
    {
        return VGet(
            x * WorldScale,
            -y * WorldScale,
            depth);
    }

#if BEGINNER_ENABLE_MODEL_SIZE_DEBUG
    /// <summary>
    /// SetModelSize2Dで設定した論理サイズと中心位置を表示する
    /// </summary>
    void DrawModelSizeDebug(float x, float y, float width, float height)
    {
        if (width <= 0.0f || height <= 0.0f)
        {
            return;
        }

        const float left   = x - width / 2.0f;
        const float right  = x + width / 2.0f;
        const float top    = y - height / 2.0f;
        const float bottom = y + height / 2.0f;

        const VECTOR leftTop = ConvertScreenToWorldPosition(
            left, top, ModelSizeDebugDepth);
        const VECTOR rightTop = ConvertScreenToWorldPosition(
            right, top, ModelSizeDebugDepth);
        const VECTOR rightBottom = ConvertScreenToWorldPosition(
            right, bottom, ModelSizeDebugDepth);
        const VECTOR leftBottom = ConvertScreenToWorldPosition(
            left, bottom, ModelSizeDebugDepth);

        const unsigned int color = GetColor(0, 255, 0);

        DrawLine3D(leftTop, rightTop, color);
        DrawLine3D(rightTop, rightBottom, color);
        DrawLine3D(rightBottom, leftBottom, color);
        DrawLine3D(leftBottom, leftTop, color);

        const VECTOR centerLeft = ConvertScreenToWorldPosition(
            x - ModelSizeDebugCenterHalfSize, y, ModelSizeDebugDepth);
        const VECTOR centerRight = ConvertScreenToWorldPosition(
            x + ModelSizeDebugCenterHalfSize, y, ModelSizeDebugDepth);
        const VECTOR centerTop = ConvertScreenToWorldPosition(
            x, y - ModelSizeDebugCenterHalfSize, ModelSizeDebugDepth);
        const VECTOR centerBottom = ConvertScreenToWorldPosition(
            x, y + ModelSizeDebugCenterHalfSize, ModelSizeDebugDepth);

        DrawLine3D(centerLeft, centerRight, color);
        DrawLine3D(centerTop, centerBottom, color);
    }
#endif
}

/// <summary>
/// 疑似2D表示に必要な3D描画設定とカメラの初期状態を設定する
/// </summary>
void Initialize2D3D()
{
    int screenWidth   = 0;
    int screenHeight  = 0;
    int colorBitDepth = 0;
    GetScreenState(&screenWidth, &screenHeight, &colorBitDepth);

    // ColorBitはカメラ計算には使わないため、画面サイズだけ利用する
    (void)colorBitDepth;

    // 3Dモデル同士の前後関係をZバッファで判定する
    SetUseZBufferFlag(TRUE);
    SetWriteZBufferFlag(TRUE);
    SetUseBackCulling(TRUE);

    // 狭いFOVにして遠くから見ることで、透視投影の歪みを小さくする
    const float halfWorldHeight = screenHeight * WorldScale * 0.5f;
    const float halfFovRadian   = Pseudo2DFieldOfViewDegree * 0.5f * (DX_PI_F / 180.0f);
    const float cameraDistance  = halfWorldHeight / std::tan(halfFovRadian);

    const VECTOR screenCenter = ConvertScreenToWorldPosition(
        screenWidth * 0.5f,
        screenHeight * 0.5f,
        0.0f);

    camera.SetFieldOfView(Pseudo2DFieldOfViewDegree);
    camera.SetNearFar(CameraNearDistance, CameraFarDistance);
    camera.ResetOffset();
    camera.SetPosition(VGet(screenCenter.x, screenCenter.y, -cameraDistance));
    camera.SetTarget(screenCenter);
    camera.Apply();
}

/// <summary>
/// 2Dゲームで扱う3Dモデルを読み込む
/// </summary>
int LoadModel2D(const char* filePath)
{
    return System::Model::Load(filePath);
}

/// <summary>
/// 3Dモデルを2Dゲーム上で扱う論理サイズを設定する
/// </summary>
void SetModelSize2D(int modelHandle, float width, float height)
{
    if (modelHandle < 0)
    {
        return;
    }

    Model2DSettings& settings = modelSettings[modelHandle];
    settings.width  = width;
    settings.height = height;
}

/// <summary>
/// 3Dモデルに設定した2Dゲーム上の論理サイズを取得する
/// </summary>
void GetModelSize2D(int modelHandle, float* width, float* height)
{
    if (width == nullptr || height == nullptr)
    {
        return;
    }

    if (modelHandle < 0)
    {
        *width = 0.0f;
        *height = 0.0f;
        return;
    }

    const auto iterator = modelSettings.find(modelHandle);
    if (iterator == modelSettings.end())
    {
        *width = 0.0f;
        *height = 0.0f;
        return;
    }

    *width = iterator->second.width;
    *height = iterator->second.height;
}

/// <summary>
/// 3Dモデルの原点と2Dゲーム上の中心位置のずれを調整する
/// 値は2D画面座標と同じ単位で、+Yは画面下方向
/// </summary>
void SetModelOffset2D(int modelHandle, float offsetX, float offsetY)
{
    if (modelHandle < 0)
    {
        return;
    }

    Model2DSettings& settings = modelSettings[modelHandle];
    settings.offsetX          = offsetX;
    settings.offsetY          = offsetY;
}

/// <summary>
/// 3Dモデル自体の表示スケールを一様に設定する
/// </summary>
void SetModelScale2D(int modelHandle, float scale)
{
    System::Model::SetScale(modelHandle, VGet(scale, scale, scale));
}

/// <summary>
/// 3Dモデルの向きを度数法で設定する
/// </summary>
void SetModelRotation2D(int modelHandle, float xDegree, float yDegree, float zDegree)
{
    const VECTOR rotationRadian = VGet(
        xDegree * (DX_PI_F / 180.0f),
        yDegree * (DX_PI_F / 180.0f),
        zDegree * (DX_PI_F / 180.0f));

    System::Model::SetRotation(modelHandle, rotationRadian);
}

/// <summary>
/// 中心座標を指定して3Dモデルを2Dゲーム上へ描画する
/// </summary>
void DrawModel2D(float x, float y, int modelHandle)
{
    // DrawModel2Dは位置だけを設定し、ScaleやRotationは変更しない
    float drawX = x;
    float drawY = y;

    // 3Dモデルの原点が足元などにある場合は、見た目の中心だけをずらす
    // 論理座標や当たり判定の中心位置には影響しない
    const auto iterator = modelSettings.find(modelHandle);
    if (iterator != modelSettings.end())
    {
        drawX += iterator->second.offsetX;
        drawY += iterator->second.offsetY;
    }

    const VECTOR worldPosition = ConvertScreenToWorldPosition(drawX, drawY, ModelDepth);
    System::Model::SetPosition(modelHandle, worldPosition);
    System::Model::Draw(modelHandle);

#if BEGINNER_ENABLE_MODEL_SIZE_DEBUG
    if (iterator != modelSettings.end())
    {
        DrawModelSizeDebug(
            x,
            y,
            iterator->second.width,
            iterator->second.height);
    }
#endif
}

/// <summary>
/// 中心座標を指定して2D画像を3D空間のQuadとして描画する
/// </summary>
void DrawGraph3D(float x, float y, int graphHandle)
{
    if (graphHandle < 0)
    {
        return;
    }

    System::WorldSprite& worldSprite = worldSprites[graphHandle];

    int graphWidth  = 0;
    int graphHeight = 0;
    GetGraphSize(graphHandle, &graphWidth, &graphHeight);

    worldSprite.SetTexture(graphHandle);
    worldSprite.SetPivot(0.5f, 0.5f);
    worldSprite.SetSize(graphWidth * WorldScale, graphHeight * WorldScale);
    worldSprite.SetPosition(ConvertScreenToWorldPosition(x, y, BackgroundDepth));

    // Beginner2D3Dでは背景画像に3Dライトを当てず、元画像の色をそのまま使う
    // この関数は描画後にDxLibの標準状態であるライティングONへ戻す
    SetUseLighting(FALSE);
    worldSprite.Draw();
    SetUseLighting(TRUE);
}
