// 2024 Takeru Yui All Rights Reserved.
#include "DxLib.h"
#include <string>
#include "Screen.h"
#include "YuiLib.h"
#include "Enemy.h"

/// <summary>
/// コンストラクタ
/// </summary>
Enemy::Enemy()
	: Collidable		(Collidable::Priority::Low, GameObjectTag::Enemy, YuiLib::ColliderData::Kind::Circle2D)
	, graphicHandle		(-1)
{
	auto circleColliderData = dynamic_cast<YuiLib::ColliderDataCircle2D*>(colliderData);
	circleColliderData->radius = 20.0f;
}

/// <summary>
/// 初期化
/// </summary>
void Enemy::Initialize(YuiLib::Physics* physics)
{
	Collidable::Initialize(physics);

	// 敵のグラフィックをメモリにロード＆表示座標を初期化
	graphicHandle = LoadGraph("data/texture/enemy.png");

	// 物理挙動の初期化
	rigidbody.Initialize();
	rigidbody.SetPos(VGet(200, 200, 0));
}

/// <summary>
/// 更新
/// </summary>
void Enemy::Update()
{
	// どこにも移動しない
	rigidbody.SetVelocity(VGet(0, 0, 0));
}

/// <summary>
/// 描画
/// </summary>
void Enemy::Draw()
{
	DrawRotaGraph(
		static_cast<int>(rigidbody.GetPos().x),
		static_cast<int>(rigidbody.GetPos().y),
		1.0f,
		0,
		graphicHandle,
		TRUE);
}

/// <summary>
/// 衝突したとき
/// </summary>
void Enemy::OnCollide(const Collidable& colider)
{
	std::string message = "敵が";
	if (colider.GetTag() == GameObjectTag::Player)
	{
		message += "プレイヤー";
	}
	else if (colider.GetTag() == GameObjectTag::Enemy)
	{
		message += "敵";
	}
	else if (colider.GetTag() == GameObjectTag::SystemWall)
	{
		message += "システム壁";
	}
	message += "と当たった！\n";
	printfDx(message.c_str());
}

