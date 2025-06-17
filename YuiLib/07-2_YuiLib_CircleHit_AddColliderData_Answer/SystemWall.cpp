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
	auto lineColliderData = static_cast<YuiLib::ColliderDataLine2D*>(colliderData);

	// 壁の位置を作る
	lineColliderData->startPoint	= VGet(600, 100, 0);
	lineColliderData->endPoint		= VGet(600, 400, 0);
}

/// <summary>
/// 初期化
/// </summary>
void SystemWall::Initialize(YuiLib::Physics* physics)
{
	physics->Entry(this);	// 物理情報に自身を登録

	// 物理挙動の初期化
	rigidbody.Initialize();
	rigidbody.SetPos(VGet(200, 200, 0));
}

/// <summary>
/// 終了
/// </summary>
void SystemWall::Finalize(YuiLib::Physics* physics)
{
	physics->Exit(this);	// 物理情報登録解除
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
	std::string message = "システム壁が";
	if (colider.GetTag() == GameObjectTag::Player)
	{
		message += "プレイヤー";
	}
	else if (colider.GetTag() == GameObjectTag::SystemWall)
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

