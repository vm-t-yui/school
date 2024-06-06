// 2024 Takeru Yui All Rights Reserved.
#include "DxLib.h"
#include "YuiLib.h"

using namespace YuiLib;

/// <summary>
/// コンストラクタ
/// </summary>
ColliderDataCircle2D::ColliderDataCircle2D()
	: ColliderData(ColliderData::Kind::Circle2D)
	, center(VGet(0, 0, 0))
	, radius(0.0f)
{
	// 処理なし
}