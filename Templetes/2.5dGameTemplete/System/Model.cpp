// 2026 Takeru Yui All rights reserved.
#include "Model.h"
#include "SystemConfig.h"

namespace
{
    /// <summary>
    /// モデルハンドルが利用可能か確認する
    /// </summary>
    bool IsValidModelHandle(int modelHandle)
    {
        if (modelHandle >= 0)
        {
            return true;
        }

#if SYSTEM_ENABLE_DEBUG_CHECK
        printfDx("[System::Model] Invalid model handle: %d\n", modelHandle);
#endif
        return false;
    }
}

namespace System::Model
{
    /// <summary>
    /// 3Dモデルを読み込む
    /// </summary>
    int Load(const char* filePath)
    {
        if (filePath == nullptr)
        {
#if SYSTEM_ENABLE_DEBUG_CHECK
            printfDx("[System::Model] File path is null\n");
#endif
            return -1;
        }

        const int modelHandle = MV1LoadModel(filePath);

#if SYSTEM_ENABLE_DEBUG_CHECK
        if (modelHandle < 0)
        {
            printfDx("[System::Model] Failed to load model: %s\n", filePath);
        }
#endif

        return modelHandle;
    }

    /// <summary>
    /// 読み込み済みの3Dモデルを複製する
    /// </summary>
    int Duplicate(int modelHandle)
    {
        if (!IsValidModelHandle(modelHandle))
        {
            return -1;
        }

        const int duplicatedModelHandle = MV1DuplicateModel(modelHandle);

#if SYSTEM_ENABLE_DEBUG_CHECK
        if (duplicatedModelHandle < 0)
        {
            printfDx("[System::Model] Failed to duplicate model: %d\n", modelHandle);
        }
#endif

        return duplicatedModelHandle;
    }

    /// <summary>
    /// 3Dモデルを削除する
    /// </summary>
    void Delete(int modelHandle)
    {
        if (!IsValidModelHandle(modelHandle))
        {
            return;
        }

        MV1DeleteModel(modelHandle);
    }

    /// <summary>
    /// 3Dモデルの位置を設定する
    /// </summary>
    void SetPosition(int modelHandle, const VECTOR& position)
    {
        if (!IsValidModelHandle(modelHandle))
        {
            return;
        }

        MV1SetPosition(modelHandle, position);
    }

    /// <summary>
    /// 3Dモデルの回転をラジアンで設定する
    /// </summary>
    void SetRotation(int modelHandle, const VECTOR& rotationRadian)
    {
        if (!IsValidModelHandle(modelHandle))
        {
            return;
        }

        MV1SetRotationXYZ(modelHandle, rotationRadian);
    }

    /// <summary>
    /// 3Dモデルのスケールを設定する
    /// </summary>
    void SetScale(int modelHandle, const VECTOR& scale)
    {
        if (!IsValidModelHandle(modelHandle))
        {
            return;
        }

        MV1SetScale(modelHandle, scale);
    }

    /// <summary>
    /// 3Dモデルを描画する
    /// </summary>
    void Draw(int modelHandle)
    {
        if (!IsValidModelHandle(modelHandle))
        {
            return;
        }

        MV1DrawModel(modelHandle);
    }
}
