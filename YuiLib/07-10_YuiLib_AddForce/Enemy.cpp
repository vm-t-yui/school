// 2024 Takeru Yui All Rights Reserved.
#include "DxLib.h"
#include <string>
#include "Screen.h"
#include "YuiLib.h"
#include "WorldSprite.h"
#include "Axe.h"
#include "Player.h"
#include "Enemy.h"

/// <summary>
/// コンストラクタ
/// </summary>
Enemy::Enemy()
	: Collidable		(Collidable::Priority::Low, GameObjectTag::Enemy, YuiLib::ColliderData::Kind::Circle2D, false)
	, graphicHandle		(-1)
	, worldSprite		(nullptr)
{
	// 重力は無視して浮遊
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
void Enemy::Update(const Player& player)
{
	// 重力は無視して浮遊
	// 一定パワーでプレイヤーを追い続ける
	if (isKnockBacing == false)
	{
		VECTOR toPlayer = VSub(player.GetRigidbody().GetPos(), rigidbody.GetPos());
		toPlayer = VNorm(toPlayer);
		rigidbody.SetVelocity(VScale(toPlayer, MoveSpeed));
	}
	// ノックバック中は、自分のVelocityにブレーキをかけ続ける
	else
	{
		// ノックバック完了したら止まる
		VECTOR velocity = rigidbody.GetVelocity();
		float velocitySize = VSize(velocity);
		if (velocitySize <= KnockbackBreakForce)
		{
			rigidbody.SetVelocity(VGet(0, 0, 0));
			isKnockBacing = false;
		}
		else
		{
			VECTOR dir = VNorm(velocity);
			rigidbody.SetVelocity(VScale(dir, velocitySize - KnockbackBreakForce));
		}
	}
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
	auto tag = colider.GetTag();
	switch (tag)
	{
	case GameObjectTag::Player:
		message += "プレイヤー";
		break;
	case GameObjectTag::Enemy:
		message += "敵";
		break;
	case GameObjectTag::Axe:
		message += "斧";

		isKnockBacing = true;

		// 斧が当たった時、斧のポジションと反対方向へ飛ぶ
		VECTOR dir = VSub(rigidbody.GetPos(), colider.GetRigidbody().GetPos());
		dir = VNorm(dir);
		rigidbody.SetVelocity(VScale(dir, KnockbackForce));

		break;
	case GameObjectTag::SystemWall:
		message += "システム壁";
		break;
	case GameObjectTag::StepGround:
		message += "足場";
		break;
	default:
		break;
	}
	message += "と当たった！\n";
	printfDx(message.c_str());
}

