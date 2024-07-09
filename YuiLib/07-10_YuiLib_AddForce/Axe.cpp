// 2024 Takeru Yui All Rights Reserved.
#include "DxLib.h"
#include <string>
#include "Screen.h"
#include "YuiLib.h"
#include "WorldSprite.h"
#include "Axe.h"
#include "Player.h"

/// <summary>
/// コンストラクタ
/// </summary>
Axe::Axe()
	: Collidable(Collidable::Priority::High, GameObjectTag::Axe, YuiLib::ColliderData::Kind::Circle2D, true)
	, graphicHandle		(-1)
	, worldSprite		(nullptr)
	, lifeCount			(LifeTimeCount)
	, isDead			(false)
{
	auto circleColliderData = dynamic_cast<YuiLib::ColliderDataCircle2D*>(colliderData);
	circleColliderData->radius = 0.25f;
	AddThroughTag(GameObjectTag::Player);
}

/// <summary>
/// 初期化
/// </summary>
void Axe::Initialize(YuiLib::Physics* physics)
{
	Collidable::Initialize(physics);

	// グラフィックをメモリにロード＆表示座標を初期化
	graphicHandle = LoadGraph("data/texture/axe.png");

	worldSprite = new YuiLib::WorldSprite();
	worldSprite->Initialize(graphicHandle, 64, 0);

	// 物理挙動の初期化
	rigidbody.Initialize(true);
}

/// <summary>
/// 終了
/// </summary>
void Axe::Finalize(YuiLib::Physics* physics)
{
	delete worldSprite;
	worldSprite = nullptr;
	Collidable::Finalize(physics);
}

/// <summary>
/// 更新
/// </summary>
void Axe::Update(YuiLib::Physics* physics)
{
	--lifeCount;

	if (lifeCount <= 0)
	{
		isDead = true;
	}
}

/// <summary>
/// 描画
/// </summary>
void Axe::Draw()
{
	worldSprite->SetTransform(rigidbody.GetPos(), 1.0f);
	worldSprite->Draw();
}

/// <summary>
/// 衝突したとき
/// </summary>
void Axe::OnCollide(const Collidable& colider)
{
	auto tag = colider.GetTag();
	switch (tag)
	{
	case GameObjectTag::Player:
		break;
	case GameObjectTag::Axe:
		break;
	case GameObjectTag::Enemy:
	case GameObjectTag::SystemWall:
	case GameObjectTag::StepGround:
		// 壁か地面か敵にぶつかったら消える
		isDead = true;
		break;
	default:
		break;
	}
}

/// <summary>
/// 撃たれたとき
/// </summary>
void Axe::OnFire(const Player& player)
{
	isDead = false;

	// プレイヤーと同じ位置に出す
	rigidbody.SetPos(player.GetRigidbody().GetPos());

	// プレイヤーの向きの斜め上に斧飛ばす
	auto shotForce = ShotForce;

	// 左向きだったら左に飛ばす
	if (player.IsRightFace() == false)
	{
		shotForce.x *= -1;
	}
	rigidbody.SetVelocity(shotForce);
}

