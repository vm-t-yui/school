// 2024 Takeru Yui All Rights Reserved.
#include "DxLib.h"
#include <string>
#include "Screen.h"
#include "YuiLib.h"
#include "WorldSprite.h"
#include "Enemy.h"

/// <summary>
/// コンストラクタ
/// </summary>
Enemy::Enemy()
	: Collidable		(Collidable::Priority::Low, GameObjectTag::Enemy, YuiLib::ColliderData::Kind::Circle2D, false)
	, graphicHandle		(-1)
	, worldSprite		(nullptr)
{
	auto circleColliderData = dynamic_cast<YuiLib::ColliderDataCircle2D*>(colliderData);
	circleColliderData->radius = 0.5f;
	AddThroughTag(GameObjectTag::StepGround);
}

/// <summary>
/// 初期化
/// </summary>
void Enemy::Initialize(YuiLib::Physics* physics)
{
	Collidable::Initialize(physics);

	// 敵のグラフィックをメモリにロード＆表示座標を初期化
	graphicHandle = LoadGraph("data/texture/enemy.png");
	worldSprite = new YuiLib::WorldSprite();
	worldSprite->Initialize(graphicHandle, 64, 0);

	// 物理挙動の初期化
	rigidbody.Initialize();
	rigidbody.SetPos(VGet(-2.0f, 4.0f, 0));
}

/// <summary>
/// 終了
/// </summary>
void Enemy::Finalize(YuiLib::Physics* physics)
{
	delete worldSprite;
	worldSprite = nullptr;
	Collidable::Finalize(physics);
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
	// エネミーの顔＝1.0mのつもりで設定
	worldSprite->SetTransform(rigidbody.GetPos(), 1.0f);
	worldSprite->Draw();
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
	else if (colider.GetTag() == GameObjectTag::StepGround)
	{
		message += "足場";
	}
	message += "と当たった！\n";
	//printfDx(message.c_str());
}

