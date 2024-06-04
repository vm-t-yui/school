// 2024 Takeru Yui All Rights Reserved.
#include "DxLib.h"
#include "Rigidbody.h"

using namespace YuiLib;

/// <summary>
/// コンストラクタ
/// </summary>
Rigidbody::Rigidbody()
	: pos		(VGet(0, 0, 0))
	, dir		(VGet(0, 0, 0))
	, velocity	(VGet(0, 0, 0))
{
	// 処理なし
}

/// <summary>
/// 初期化
/// </summary>
void Rigidbody::Initialize()
{
	pos = VGet(0, 0, 0);
	dir = VGet(0, 0, 0);
	velocity = VGet(0, 0, 0);
}

/// <summary>
/// VelocityのSetとDir自動計算
/// </summary>
void Rigidbody::SetVelocity(const VECTOR& set)
{
	velocity = set;
	if (VSquareSize(velocity) > 0)
	{
		dir = VNorm(velocity);
	}
}