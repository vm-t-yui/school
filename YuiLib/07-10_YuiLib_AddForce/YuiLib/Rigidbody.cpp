// 2024 Takeru Yui All Rights Reserved.
#include "DxLib.h"
#include "Rigidbody.h"

using namespace YuiLib;

/// <summary>
/// コンストラクタ
/// </summary>
Rigidbody::Rigidbody()
	: pos		 (VGet(0, 0, 0))
	, dir		 (VGet(0, 0, 0))
	, velocity	 (VGet(0, 0, 0))
	, useGravity (false)
{
	// 処理なし
}

/// <summary>
/// 初期化
/// </summary>
void Rigidbody::Initialize(bool useGravity)
{
	pos = VGet(0, 0, 0);
	dir = VGet(0, 0, 0);
	velocity = VGet(0, 0, 0);
	this->useGravity = useGravity;
}

/// <summary>
/// velocityに力を加える
/// </summary>
void Rigidbody::AddForce(const VECTOR& force)
{
	velocity = VAdd(velocity, force);
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