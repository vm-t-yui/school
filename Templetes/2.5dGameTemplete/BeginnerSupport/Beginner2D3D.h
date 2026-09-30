// 2026 Takeru Yui All rights reserved.
#pragma once

// モデルの論理サイズを確認するためのデバッグ表示
// 3Dモデル本来の大きさや当たり判定ではなく、SetModelSize2Dで設定した
// 2Dゲーム上の幅・高さと中心位置を表示する
#ifndef BEGINNER_ENABLE_MODEL_SIZE_DEBUG
#define BEGINNER_ENABLE_MODEL_SIZE_DEBUG 0
#endif

// ============================================================================
// Beginner2D3D
//
// 2Dゲームとほぼ同じ考え方で3Dモデルを扱うための教材用中間層
// 初学者はこのファイルを変更する必要はない
//
// この中間層は本格的な3Dゲーム用フレームワークではない
// 必要になった機能だけSystem以下から直接利用してもよく、
// 本格的な3Dゲームへ進む場合はこの中間層を削除して作り直してもよい
//
// このテンプレートでは左上を(0, 0)、右を+X、下を+Yとする画面座標を使う
// ただし3DモデルとWorldSpriteの描画位置は中心座標で指定する
// ============================================================================

/// <summary>
/// 疑似2D表示に必要な3D描画設定とカメラの初期状態を設定する
/// </summary>
void Initialize2D3D();

/// <summary>
/// 2Dゲームで扱う3Dモデルを読み込む
/// </summary>
int LoadModel2D(const char* filePath);

/// <summary>
/// 3Dモデルを2Dゲーム上で扱う論理サイズを設定する
/// </summary>
void SetModelSize2D(int modelHandle, float width, float height);

/// <summary>
/// 3Dモデルに設定した2Dゲーム上の論理サイズを取得する
/// </summary>
void GetModelSize2D(int modelHandle, float* width, float* height);

/// <summary>
/// 3Dモデルの原点と2Dゲーム上の中心位置のずれを調整する
/// 値は2D画面座標と同じ単位で、+Yは画面下方向
/// </summary>
void SetModelOffset2D(int modelHandle, float offsetX, float offsetY);

/// <summary>
/// 3Dモデル自体の表示スケールを一様に設定する
/// </summary>
void SetModelScale2D(int modelHandle, float scale);

/// <summary>
/// 3Dモデルの向きを度数法で設定する
/// </summary>
void SetModelRotation2D(int modelHandle, float xDegree, float yDegree, float zDegree);

/// <summary>
/// 中心座標を指定して3Dモデルを2Dゲーム上へ描画する
/// </summary>
void DrawModel2D(float x, float y, int modelHandle);

/// <summary>
/// 中心座標を指定して2D画像を3D空間のQuadとして描画する
/// </summary>
void DrawGraph3D(float x, float y, int graphHandle);
