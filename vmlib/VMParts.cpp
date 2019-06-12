//-----------------------------------------------------------------------------
// @brief  VMメッシュのパーツクラス.
// Copyright (c) 2017 VikingMaxx Takeru Yui. All rights reserved.
//-----------------------------------------------------------------------------
#include "vmlib.h"
#include "VMParts.h"
#include "VMMesh.h"

// メッシュ関連のログ表示.
#define LOG_MESH FALSE
// 埋め込みテクスチャの有効化.
#define ENABLE_IMP_TEXTURE FALSE

// Our custom FVF, which describes our custom vertex structure
#define VM_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

//-----------------------------------------------------------------------------
// @brief コンストラクタ.
//-----------------------------------------------------------------------------
VMParts::VMParts(VMMesh* owner, FbxMesh* mesh)
{
    ownerMesh = owner;
    polygonCount = mesh->GetPolygonCount();         // ポリゴン数を取得.
    vertexCount = mesh->GetControlPointsCount();    // 総頂点数を取得.

    // Create the vertex buffer.
    if (FAILED(GetDxDevice()->CreateVertexBuffer(
        vertexCount * sizeof(CUSTOMVERTEX),
        0, VM_CUSTOMVERTEX,
        D3DPOOL_DEFAULT,
        &d3dVertexBuffer,
        NULL)))
    {
        LogAssert("vertex bufferの作成に失敗しました。\n");
    }
    else
    {
        // Fill the vertex buffer.
        if (FAILED(d3dVertexBuffer->Lock(0, 0, (void**)&vertices, 0)))
        {
            LogAssert("vertex bufferのロックに失敗しました。\n");
        }
        else
        {
            // 頂点座標のキャストとハードコピー.
            FbxVector4* vertex = mesh->GetControlPoints();
#if LOG_MESH
            std::string str = "";
#endif
            for (int i = 0; vertexCount > i; i++)
            {
                // i番目の頂点の座標Xを取得.
                vertices[i].position.x = (float)vertex[i][0];

                // i番目の頂点の座標Yを取得.
                vertices[i].position.y = (float)vertex[i][1];

                // i番目の頂点の座標Xを取得.
                vertices[i].position.z = (float)vertex[i][2];

                // 頂点カラー.
                vertices[i].color = 0xffffffff;

#if LOG_MESH
                str += "[";
                str += std::to_string(i);
                str += "]";
                str += "x:";
                str += std::to_string(vertices[i].position.x);
                str += " ";
                str += "y:";
                str += std::to_string(vertices[i].position.y);
                str += " ";
                str += "z:";
                str += std::to_string(vertices[i].position.z);
                str += "\n";
#endif
            }
#if LOG_MESH
            Log("vertices======================\n");
            Log(str.c_str());
            Log("==============================\n");
#endif
            d3dVertexBuffer->Unlock();
        }
    }

    // インデックスバッファの要素数を取得
    polygonSizes = new int[polygonCount];
    UINT IndexesCount = 0;
    for (int i = 0; i<polygonCount; i++)
    {
        // 1ポリゴン内の頂点数を取得
        polygonSizes[i] = mesh->GetPolygonSize(i);

        switch (polygonSizes[i])
        {
        case 3:  // 三角ポリゴン
            indexCount += 3;
            break;
        case 4:  // 四角ポリゴン
                 // 四角ポリゴンの場合三角ポリゴンに変換する
            indexCount += 6;
            break;
        default:
            // ５角形以上のパターンがあるけど、対応しない
            LogAssert("5角形以上の多角形ポリゴンはサポートしていません lPolygonSize:%d\n", polygonSizes[i]);
            break;
        }
    }

    if (FAILED(GetDxDevice()->CreateIndexBuffer(indexCount * sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16,
        D3DPOOL_DEFAULT, &d3dIndexBuffer, NULL)))
    {
        LogAssert("index bufferの作成に失敗しました。\n");
    }
    else
    {

        if (FAILED(d3dIndexBuffer->Lock(0,         // Fill from start of the buffer.
            0,                                  // Size of the data to load.
            (VOID**)&indices,                        // Returned index data.
            0)))                                // Send default flags to the lock.
        {
            LogAssert("vertex bufferのロックに失敗しました。\n");
        }
        else
        {
            // インデックスバッファのハードコピー.
            UINT index = 0;
            UINT indexArray[6] = { 0, 1, 2, 0, 2, 3 };
            for (int i = 0; i<polygonCount; i++)
            {
                // 1ポリゴン内の頂点数を取得
                int lPolygonSize = mesh->GetPolygonSize(i);

                switch (lPolygonSize)
                {
                case 3:  // 三角ポリゴン
                    for (int j = 0; j<3; j++)
                    {
                        // コントロールポイントのインデックスを取得
                        indices[index] = mesh->GetPolygonVertex(i, j);
                        index++;
                    }
                    break;
                case 4: // 四角ポリゴン
                    for (int j = 0; j<6; j++)
                    {
                        indices[index] = mesh->GetPolygonVertex(i, indexArray[j]);
                        index++;
                    }
                    break;
                }
            }
#if LOG_MESH
            std::string str = "";
#endif
            for (int i = 0; indexCount > i; i++)
            {
#if LOG_MESH
                str += "[";
                str += std::to_string(i);
                str += "]";
                str += ":";
                str += std::to_string(indices[i]);
                str += "\n";
#endif
            }
#if LOG_MESH
            Log("indices=======================\n");
            Log(str.c_str());
            Log("==============================\n");
#endif
            d3dIndexBuffer->Unlock();
        }
    }
    // レイヤー数が 2 以上はサポート外.
    int layerCount = mesh->GetLayerCount();
    if (layerCount > 1)
    {
        LogWarn("レイヤー数が2以上はサポート外です。layerCount:%d\n", layerCount);
    }

    normalLayerCount = mesh->GetElementNormalCount();   // 法線のセット数.

    for (int i = 0; normalLayerCount > i; i++)
    {
        // 法線セットを取得。
        FbxGeometryElementNormal* normal = mesh->GetElementNormal(i);
        // マッピングモードの取得
        FbxGeometryElement::EMappingMode mapping = normal->GetMappingMode();
        // リファレンスモードの取得。
        FbxGeometryElement::EReferenceMode reference = normal->GetReferenceMode();

        // マッピングモードの判別。
        switch (mapping)
        {
            // ポリゴン単位で頂点情報がセットされている
        case FbxLayerElement::eByPolygonVertex:
            switch (reference)
            {
                // インデックスバッファを使用せず、直接法線ベクトルが格納されている
            case FbxLayerElement::eDirect:
            {
                // 頂点ごとに対応する法線ベクトルを探す
                for (int i = 0; i < vertexCount; i++)
                {
                    D3DXVECTOR3 Vec3 = D3DXVECTOR3(0, 0, 0);
                    UINT NormalIndex = 0;

                    // 法線ベクトルの重複チェック用のバッファ
                    std::vector<D3DXVECTOR3> NormalTempArray;
                    NormalTempArray.clear();

                    // ポリゴンごとのループ
                    for (int j = 0; j < polygonCount; j++)
                    {
                        // 1ポリゴン内の頂点数を取得
                        int lPolygonSize = mesh->GetPolygonSize(j);
                        for (int k = 0; k < lPolygonSize; k++)
                        {
                            // インデックスが同じなので処理対象
                            if (i == mesh->GetPolygonVertex(j, k))
                            {
                                // 法線ベクトル取得
                                FbxVector4 vec4 = normal->GetDirectArray().GetAt(NormalIndex);

                                bool Find = false;
                                for (int l = 0; l < (int)NormalTempArray.size(); l++)
                                {
                                    // すでに加算処理すみの法線ベクトルかチェックする
                                    if (NormalTempArray[l].x == vec4[0] &&
                                        NormalTempArray[l].y == vec4[1] &&
                                        NormalTempArray[l].z == vec4[2])
                                    {
                                        Find = true;
                                        break;
                                    }
                                }
                                // 法線ベクトルが完全に一致する頂点を再度加算処理することがあるので、
                                // 重複して加算しないようにする
                                if (Find == false)
                                {
                                    NormalTempArray.push_back(D3DXVECTOR3((float)vec4[0], (float)vec4[1], (float)vec4[2]));

                                    Vec3 = D3DXVECTOR3(
                                        Vec3.x + (float)vec4[0],
                                        Vec3.y + (float)vec4[1],
                                        Vec3.z + (float)vec4[2]);
                                }
                            }

                            NormalIndex++;
                        }
                    }

                    // 単位ベクトルの計算
                    D3DXVec3Normalize(&Vec3, &Vec3);

                    // 法線ベクトルをセット
                    ::CopyMemory(&(vertices[i].normal), &Vec3, sizeof(D3DXVECTOR3));
                }
            }
            break;

            // インデックス参照で法線ベクトルが格納されている
            // サンプルにここの処理が実装されているので処理されるパターンが存在すると思われるが、確認できなかった
            case FbxLayerElement::eIndexToDirect:
            {
                LogWarn("法線ベクトルでeDIRECTでeINDEX_TO_DIRECT は使用できません.\n");
            }
            }
            break;

            // 頂点単位で頂点情報がセットされている
        case FbxLayerElement::eByControlPoint:
            if (normal->GetReferenceMode() == FbxLayerElement::eDirect)
            {
                for (int i = 0; i < vertexCount; i++)
                {
                    FbxVector4 vec4 = normal->GetDirectArray().GetAt(i);
                    vertices[i].normal.x = (float)vec4[0];
                    vertices[i].normal.y = (float)vec4[1];
                    vertices[i].normal.z = (float)vec4[2];
                }
            }
            // サンプルにここの処理は実装されていない。
            else
            {
                LogWarn("法線ベクトルでeBY_CONTROL_POINTでeDIRECT以外はサポートされません.\n");
            }
            break;

        default:
            LogWarn("不明な法線ベクトルデータが存在します。\n");
        }
    }

    // UVを取得
    int UVLayerCount = mesh->GetElementUVCount();

    // UVレイヤーは一つだけサポート.
    if (UVLayerCount > 1)
    {
        LogWarn("UVレイヤーは一つだけしかサポートしていません。layerNum:%d\n", UVLayerCount);
    }

    // UVバッファを取得
    FbxGeometryElementUV* UV = mesh->GetElementUV(0);   // UVレイヤーは一つだけ.

    // マッピングモードの判別
    if (UV)
    {
        // マッピングモードの取得
        FbxGeometryElement::EMappingMode mapping = UV->GetMappingMode();
        // リファレンスモードの取得
        FbxGeometryElement::EReferenceMode reference = UV->GetReferenceMode();

        // UV数を取得
        int uvCount = UV->GetDirectArray().GetCount();

        switch (UV->GetMappingMode())
        {
            // ポリゴン単位で頂点情報がセットされている
        case FbxLayerElement::eByPolygonVertex:
            switch (UV->GetReferenceMode())
            {
                // このパターンは確認できず
            case FbxLayerElement::eDirect:
                LogWarn("UVでeBY_POLYGON_VERTEXでeDIRECT が処理されちゃった\n");
                break;

                // インデックス参照でuvが格納されている
            case FbxLayerElement::eIndexToDirect:
            {
                // 頂点バッファのループ
                for (int i = 0; i < vertexCount; i++)
                {
                    UINT UVIndex = 0;
                    // ポリゴンごとのループ
                    for (int j = 0; j < polygonCount; j++)
                    {
                        // ポリゴン数を取得
                        int lPolygonSize = polygonSizes[j];
                        // １ポリゴン内の頂点ごとのループ
                        for (int k = 0; k < lPolygonSize; k++)
                        {
                            // インデックスが同じなので処理対象
                            if (i == mesh->GetPolygonVertex(j, k))
                            {
                                // インデックスバッファからインデックスを取得する
                                int lUVIndex = UV->GetIndexArray().GetAt(UVIndex);

                                // 取得したインデックスから UV を取得する
                                FbxVector2 lVec2 = UV->GetDirectArray().GetAt(lUVIndex);
                                vertices[i].uv.x = (float)lVec2[0];
                                vertices[i].uv.y = (float)lVec2[1];
                            }
                            UVIndex++;
                        }
                    }
                }
            }
            }
            break;

            // 3DS MAX の場合、法線を頂点ごとに分割して出力するとここが処理される
        case FbxLayerElement::eByControlPoint:
            switch (UV->GetReferenceMode())
            {
                // インデックスバッファを使用せず、直接uvが格納されている
            case FbxLayerElement::eDirect:
                for (int i = 0; i < vertexCount; i++)
                {
                    FbxVector2 lVec2 = UV->GetDirectArray().GetAt(i);
                    vertices[i].uv.x = (float)lVec2[0];
                    vertices[i].uv.y = (float)lVec2[1];
                }
                break;
                // 確認できず
            case FbxLayerElement::eIndexToDirect:
                LogWarn("UVでeBY_CONTROL_POINTでeINDEX_TO_DIRECTが処理されちゃった\n");
                break;

            default:
                LogWarn("UVでeBY_CONTROL_POINTでeDIRECTでもeINDEX_TO_DIRECTでもない処理されちゃった\n");
            }
            break;

        default:
            LogWarn("不明なUVデータ\n");
        }
    }

#if ENABLE_IMP_TEXTURE
    // メッシュからノードを取得.
    FbxNode* node = mesh->GetNode();

    // マテリアルの数を取得.
    int materialCount = node->GetMaterialCount();

    // マテリアルの数だけ繰り返す.
    for (int i = 0; materialCount > i; i++) 
    {
        // マテリアルを取得.
        FbxSurfaceMaterial* material = node->GetMaterial(i);
        FbxProperty prop = material->FindProperty(FbxSurfaceMaterial::sDiffuse);

        // FbxLayeredTexture の数を取得.
        int layeredTextureCount = prop.GetSrcObjectCount<FbxLayeredTexture>();

        // アタッチされたテクスチャが FbxLayeredTexture の場合.
        if (0 < layeredTextureCount) 
        {
            // アタッチされたテクスチャの数だけ繰り返す.
            for (int j = 0; layeredTextureCount > j; j++) 
            {
                // テクスチャを取得.
                FbxLayeredTexture* layeredTexture = prop.GetSrcObject<FbxLayeredTexture>(j);
                // レイヤー数を取得.
                int textureCount = layeredTexture->GetSrcObjectCount<FbxFileTexture>();

                // レイヤー数だけ繰り返す.
                for (int k = 0; textureCount > k; k++) {
                    // テクスチャを取得.
                    FbxFileTexture* texture = prop.GetSrcObject<FbxFileTexture>(k);

                    if (texture) {
                        // テクスチャ名を取得.
                        //std::string textureName = texture->GetName();
                        std::string textureName = texture->GetRelativeFileName();

                        // UVSet名を取得.
                        std::string UVSetName = texture->UVSet.Get().Buffer();

                        // UVSet名を比較し対応しているテクスチャなら保持.
                        if (uvSet.uvSetName == UVSetName) {
                            uvSet.textures.push_back(textureName);
                        }
                    }
                }
            }
        }
        else 
        {
            // テクスチャ数を取得.
            int fileTextureCount = prop.GetSrcObjectCount<FbxFileTexture>();

            if (0 < fileTextureCount)
            {
                // テクスチャの数だけ繰り返す.
                for (int j = 0; fileTextureCount > j; j++)
                {
                    // テクスチャを取得.
                    FbxFileTexture* texture = prop.GetSrcObject<FbxFileTexture>(j);
                    if (texture)
                    {
                        // テクスチャ名を取得.
                        std::string textureName = texture->GetName();
                        //std::string textureName = texture->GetRelativeFileName();   // 文字化けするときはこっち.

                        // UVSet名を取得.
                        std::string UVSetName = texture->UVSet.Get().Buffer();

                        // UVSet名を比較し対応しているテクスチャなら保持.
                        if (uvSet.uvSetName == UVSetName)
                        {
                            uvSet.textures.push_back(textureName);
                        }
                    }
                }
            }
        }
    }
#endif

    //// TEXTURE
    //// このサンプルでは同じ種類のテクスチャーは１つのみ使用可能とする。
    //// テクスチャーの種類については、FBXSDKのサンプルソースの examples\ImportScene\DisplayMesh.cxx 参照
    //// またポリゴン単位でどのテクスチャーを参照するかできそうな雰囲気だが考慮しない
    //FbxLayerElementMaterial* layerMaterial = mesh->GetLayer(0)->GetMaterials();
    //if (layerMaterial)
    //{
    //    switch (layerMaterial->GetMappingMode())
    //    {
    //        // １つのメッシュに１つだけマッピングされている
    //    case FbxLayerElement::eAllSame:
    //    {
    //        int materialId = layerMaterial->GetIndexArray().GetAt(0);
    //        FbxSurfaceMaterial* material = mesh->GetNode()->GetMaterial(materialId);

    //        if (materialId >= 0)
    //        {
    //            // テクスチャーのロード
    //            GetBaseTextureMap(material);
    //        }
    //    }
    //    break;

    //    // ポリゴンごとにマッピングされている。
    //    case FbxLayerElement::eByPolygon:
    //        for (int i = 0; i<polygonCount; i++)
    //        {
    //            for (int j = 0; j<mesh->GetLayerCount(); j++)
    //            {
    //                FbxLayerElementMaterial* layerMaterial = mesh->GetLayer(j)->GetMaterials();
    //                if (layerMaterial)
    //                {
    //                    FbxSurfaceMaterial* material = NULL;
    //                    int materialId = -1;
    //                    material = mesh->GetNode()->GetMaterial(layerMaterial->GetIndexArray().GetAt(i));
    //                    materialId = layerMaterial->GetIndexArray().GetAt(i);

    //                    if (materialId >= 0)
    //                    {
    //                        // テクスチャーのロード
    //                        GetBaseTextureMap(material);
    //                    }
    //                }
    //            }
    //        }
    //        break;
    //    }
    //}


    //// FBXでは右手系座標となっているためDirect3Dで使用できるように座標系を変換する
    //for (int i = 0; i<vertexCount; i++)
    //{
    //    vertices[i].position.z *= -1.0f;
    //    vertices[i].normal.z *= -1.0f;
    //    vertices[i].uv.y = 1.0f - vertices[i].uv.y;
    //}
}
//
//void VMParts::GetBaseTextureMap(FbxSurfaceMaterial* material)
//{
//    // 拡散反射マップを取得
//    GetTextureMap(material, FbxSurfaceMaterial::sDiffuse);
//
//    // TODO:ほかの種類のテクスチャーを追加する．．．
//}
//
//void VMParts::GetTextureMap(FbxSurfaceMaterial* material, const char* materialName)
//{
//    TCHAR DebugStr[512];
//
//    FbxProperty fbxProperty = material->FindProperty(materialName);
//
//    //D3DX9_IMAGE_LOAD_INFO info;
//    //::ZeroMemory(&info, sizeof(D3DX9_IMAGE_LOAD_INFO));
//    //info.Width = D3DX9_DEFAULT;
//    //info.Height = D3DX9_DEFAULT;
//    //info.Depth = D3DX9_DEFAULT;
//    //info.FirstMipLevel = D3DX9_DEFAULT;          // テクスチャーの最高解像度のミップマップ レベル。実際の使用方法不明。
//    //info.MipLevels = 0;                           // ミップマップ数。0 または D3DX9_DEFAULT を使用するとすべてのミップマップ チェーンを作成する。
//    //info.Usage = D3D11_USAGE_DEFAULT;
//    //info.BindFlags = D3D11_BIND_SHADER_RESOURCE;
//    //info.CpuAccessFlags = 0;
//    //info.MiscFlags = 0;
//    //info.Format = DXGI_FORMAT_FROM_FILE;
//    //info.Filter = D3DX9_FILTER_POINT;            // テクスチャー読み込み時に使用するフィルター
//    //info.MipFilter = D3DX9_FILTER_POINT;         // ミップマップ作成時に使用するフィルター
//    //info.pSrcInfo = NULL;
//
//    // TODO:すでに同じ種類のテクスチャーをセットしたかをチェックする
//
//    // テクスチャ設定.
//    int layeredTextureCount = fbxProperty.GetSrcObjectCount<FbxLayeredTexture>();
//    if (layeredTextureCount == 0)
//    {
//        int nbTextures = fbxProperty.GetSrcObjectCount<FbxTexture>();
//
//        if (nbTextures == 1)
//        {
//            FbxTexture* lTexture = fbxProperty.GetSrcObject<FbxTexture>();
//            if (lTexture)
//            {
//                // TODO:テクスチャ情報をいろいろ設定できる.
//                //MESH_USER::TEXTURE_USER tex;
//                //tex.MaterialName[0] = NULL;
//                //tex.TexturePath[0] = NULL;
//                //tex.Texture = NULL;
//                //// 画像ファイルへの相対パスを取得する
//                //ConvertCHARToTCHAR(tex.TexturePath, (char*)lTexture->GetRelativeFileName());
//
//                //// テクスチャーの種類を取得
//                //ConvertCHARToTCHAR(tex.MaterialName, fbxProperty.GetName().Buffer());
//
//                //// シェーダーリソースビューの作成
//                //hr = D3DX9CreateShaderResourceViewFromFile(m_D3DDevice, tex.TexturePath, &info, NULL, &(tex.Texture), NULL);
//                //if (FAILED(hr))
//                //{
//                //    LogAssert("ERROR: テクスチャーロードエラー. TexturePath: [ %s ]\n"), tex.TexturePath));
//                //}
//                //pMeshUser->Texture.push_back(tex);
//            }
//        }
//
//        // 2以上になるパターンがわからん
//        else if (nbTextures > 1)
//        {
//            LogAssert("テクスチャー数が２個以上存在した。\n");
//        }
//    }
//    // レイヤードテクスチャーが存在するパターンの FBX ファイルの作成方法がわからん
//    else
//    {
//        LogAssert("レイヤードテクスチャーが見つかった。\n");
//    }
//}

//-----------------------------------------------------------------------------
// @brief デストラクタ.
//-----------------------------------------------------------------------------
VMParts::~VMParts()
{
    if (d3dVertexBuffer != NULL)
    {
        d3dVertexBuffer->Release();
        d3dVertexBuffer = NULL;
    }
    if (d3dIndexBuffer != NULL)
    {
        d3dIndexBuffer->Release();
        d3dIndexBuffer = NULL;
    }
    if (polygonSizes != NULL)
    {
        delete[] polygonSizes;
        polygonSizes = NULL;
    }
}

//-----------------------------------------------------------------------------
// @brief  レンダリング.
//-----------------------------------------------------------------------------
void VMParts::Render()
{
    const LPDIRECT3DDEVICE9 d3Device = GetDxDevice();
    // とりあえず1mesh1texで.
    const LPDIRECT3DTEXTURE9 tex = ownerMesh->GetTexture()->GetTexture();
    if (tex)
    {
        d3Device->SetTexture(0, tex);
        d3Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
        d3Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
        d3Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
        d3Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
    }
    d3Device->SetIndices(d3dIndexBuffer);
    d3Device->SetFVF(VM_CUSTOMVERTEX);
    d3Device->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, vertexCount, indexCount / 3, &indices[0], D3DFMT_INDEX16, &vertices[0], sizeof(CUSTOMVERTEX));
}

/*
// インデックスバッファを使用する場合の頂点情報をセット
HRESULT FBXSDK_MESHLOADER_USER::SetVertexBuffer(FbxMesh* mesh, MESH_USER* pMeshUser)
{
    // TEXTURE
    {
        // このサンプルでは同じ種類のテクスチャーは１つのみ使用可能とする。
        // テクスチャーの種類については、FBXSDKのサンプルソースの examples\ImportScene\DisplayMesh.cxx 参照
        // またポリゴン単位でどのテクスチャーを参照するかできそうな雰囲気だが考慮しない

        FbxLayerElementMaterial* layerMaterial = mesh->GetLayer(0)->GetMaterials();
        if (layerMaterial)
        {
            switch (layerMaterial->GetMappingMode())
            {
                // １つのメッシュに１つだけマッピングされている
            case FbxLayerElement::eALL_SAME:
            {
                int materialId = layerMaterial->GetIndexArray().GetAt(0);
                FbxSurfaceMaterial* material = mesh->GetNode()->GetMaterial(materialId);

                if (materialId >= 0)
                {
                    // テクスチャーのロード
                    GetBaseTextureMap(material, pMeshUser);
                }
            }
            break;

            // ポリゴンごとにマッピングされている。
            case FbxLayerElement::eBY_POLYGON:
                for (int i = 0; i<polygonCount; i++)
                {
                    for (int j = 0; j<mesh->GetLayerCount(); j++)
                    {
                        FbxLayerElementMaterial* layerMaterial = mesh->GetLayer(j)->GetMaterials();
                        if (layerMaterial)
                        {
                            FbxSurfaceMaterial* material = NULL;
                            int materialId = -1;
                            material = mesh->GetNode()->GetMaterial(layerMaterial->GetIndexArray().GetAt(i));
                            materialId = layerMaterial->GetIndexArray().GetAt(i);

                            if (materialId >= 0)
                            {
                                // テクスチャーのロード
                                GetBaseTextureMap(material, pMeshUser);
                            }
                        }
                    }
                }
                break;
            }
        }
    }

    // FBXでは右手系座標となっているためDirect3Dで使用できるように座標系を変換する
    {
        for (int i = 0; i<VertexesCount; i++)
        {
            pMeshUser->Vertexes[i].Vertex.z *= -1.0f;
            pMeshUser->Vertexes[i].Normal.z *= -1.0f;
            pMeshUser->Vertexes[i].UV.y = 1.0f - pMeshUser->Vertexes[i].UV.y;
        }
    }

    // 頂点情報をデバッガに出力
#if defined(DEBUG) || defined(_DEBUG)
    TCHAR s[512];
    _stprintf_s(s, _T("■□■ 頂点情報出力[ %d ]\n"), pMeshUser->VertexesCount);
    OutputDebugString(s);
    for (UINT j = 0; j<pMeshUser->VertexesCount; j++)
    {
        _stprintf_s(s, _T("■□■ Vertex[ %f,%f,%f ]　Normal[ %f,%f,%f ]　UV[ %f, %f ]\n"),
            pMeshUser->Vertexes[j].Vertex.x,
            pMeshUser->Vertexes[j].Vertex.y,
            pMeshUser->Vertexes[j].Vertex.z,
            pMeshUser->Vertexes[j].Normal.x,
            pMeshUser->Vertexes[j].Normal.y,
            pMeshUser->Vertexes[j].Normal.z,
            pMeshUser->Vertexes[j].UV.x,
            pMeshUser->Vertexes[j].UV.y
            );
        OutputDebugString(s);
    }

    _stprintf_s(s, _T("■□■ インデックスバッファ情報出力[ %d ]\n"), pMeshUser->IndexesCount);
    OutputDebugString(s);
    for (UINT j = 0; j<pMeshUser->IndexesCount; j++)
    {
        _stprintf_s(s, _T("■□■ Indexes[ %d ]\n"),
            pMeshUser->Indexes[j]
            );
        OutputDebugString(s);
    }

    _stprintf_s(s, _T("■□■ マテリアル情報出力[ %d ]\n"), pMeshUser->Texture.size());
    OutputDebugString(s);
    for (UINT j = 0; j<pMeshUser->Texture.size(); j++)
    {
        _stprintf_s(s, _T("■□■ Texture[ %s, %s ]\n"),
            pMeshUser->Texture[j].MaterialName,
            pMeshUser->Texture[j].TexturePath
            );
        OutputDebugString(s);
    }
#endif

    hr = S_OK;
EXIT:
    return hr;
}
*/