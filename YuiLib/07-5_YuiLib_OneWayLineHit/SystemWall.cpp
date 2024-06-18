// 2024 Takeru Yui All Rights Reserved.
#include "DxLib.h"
#include <string>
#include "Screen.h"
#include "YuiLib.h"
#include "SystemWall.h"

/// <summary>
/// コンストラクタ
/// </summary>
SystemWall::SystemWall()
	: Collidable		(Collidable::Priority::Static, GameObjectTag::SystemWall, YuiLib::ColliderData::Kind::Line2D)
{
	auto lineColliderData = dynamic_cast<YuiLib::ColliderDataLine2D*>(colliderData);

	// 壁の位置を作る
	lineColliderData->startPoint	= VGet(5.0f, 3.0f, 0);
	lineColliderData->endPoint		= VGet(5.0f, -3.0f, 0);
}

/// <summary>
/// 初期化
/// </summary>
void SystemWall::Initialize(YuiLib::Physics* physics)
{
	Collidable::Initialize(physics);

	// 物理挙動の初期化
	rigidbody.Initialize();

	// 動かないので物理的な位置は無視。仮で始点にする
	auto lineColliderData = dynamic_cast<YuiLib::ColliderDataLine2D*>(colliderData);
	rigidbody.SetPos(lineColliderData->startPoint);
}


/// <summary>
/// 更新
/// </summary>
void SystemWall::Update()
{
	// どこにも移動しない
	rigidbody.SetVelocity(VGet(0, 0, 0));
}

/// <summary>
/// 衝突したとき
/// </summary>
void SystemWall::OnCollide(const Collidable& colider)
{
	std::string message = "地面が";
	if (colider.GetTag() == GameObjectTag::Player)
	{
		message += "プレイヤー";
	}
	else if (colider.GetTag() == GameObjectTag::Enemy)
	{
		message += "敵";
	}
	else
	{
		// 壁同士は当たらないので無視
	}
	message += "と当たった！\n";
	printfDx(message.c_str());
}

