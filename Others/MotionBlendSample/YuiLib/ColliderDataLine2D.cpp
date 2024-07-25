// 2024 Takeru Yui All Rights Reserved.
#include "DxLib.h"
#include "YuiLib.h"

using namespace YuiLib;

/// <summary>
/// コンストラクタ
/// </summary>
ColliderDataLine2D::ColliderDataLine2D(bool isTrigger)
	: ColliderData(ColliderData::Kind::Line2D, isTrigger)
	, startPoint(VGet(0, 0, 0))
	, endPoint(VGet(0, 0, 0))
{
	// 処理なし
}

/// <summary>
/// コンストラクタ（継承用）
/// </summary>
ColliderDataLine2D::ColliderDataLine2D(ColliderData::Kind kind, bool isTrigger)
	: ColliderData(kind, isTrigger)
	, startPoint(VGet(0, 0, 0))
	, endPoint(VGet(0, 0, 0))
{
	// 処理なし
}