// 2024 Takeru Yui All Rights Reserved.
#include "DxLib.h"
#include "YuiLib.h"

using namespace YuiLib;

/// <summary>
/// コンストラクタ
/// </summary>
ColliderDataLine2D::ColliderDataLine2D()
	: ColliderData(ColliderData::Kind::Line2D)
	, startPoint(VGet(0, 0, 0))
	, endPoint(VGet(0, 0, 0))
{
	// 処理なし
}

/// <summary>
/// コンストラクタ（継承用）
/// </summary>
ColliderDataLine2D::ColliderDataLine2D(ColliderData::Kind kind)
	: ColliderData(kind)
	, startPoint(VGet(0, 0, 0))
	, endPoint(VGet(0, 0, 0))
{
	// 処理なし
}