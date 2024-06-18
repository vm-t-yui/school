// 2024 Takeru Yui All Rights Reserved.
#include "DxLib.h"
#include <string>
#include "Screen.h"
#include "YuiLib.h"
#include "WorldSprite.h"
#include "Player.h"

/// <summary>
/// コンストラクタ
/// </summary>
Player::Player()
	: Collidable(Collidable::Priority::High, GameObjectTag::Player, YuiLib::ColliderData::Kind::Circle2D)
	, graphicHandle		(-1)
	, speed				(0)
	, worldSprite		(nullptr)
{
	auto circleColliderData = dynamic_cast<YuiLib::ColliderDataCircle2D*>(colliderData);
	circleColliderData->radius = 0.5f;
}

/// <summary>
/// 初期化
/// </summary>
void Player::Initialize(YuiLib::Physics* physics)
{
	Collidable::Initialize(physics);

	// プレイヤーのグラフィックをメモリにロード＆表示座標を初期化
	graphicHandle = LoadGraph("data/texture/player.png");

	worldSprite = new YuiLib::WorldSprite();
	worldSprite->Initialize(graphicHandle, 64, 0);

	// 物理挙動の初期化
	rigidbody.Initialize();
	rigidbody.SetPos(VGet(0, 3, 0));
	speed = 0.1f;
}

/// <summary>
/// 終了
/// </summary>
void Player::Finalize(YuiLib::Physics* physics)
{
	delete worldSprite;
	worldSprite = nullptr;
	Collidable::Finalize(physics);
}

/// <summary>
/// 更新
/// </summary>
void Player::Update()
{
	// 矢印キーを押していたらプレイヤーを移動させる
	// 向きを決定する.
	VECTOR dir = VGet(0, 0, 0);
	dir.x = 0;
	dir.y = 0;
	if (CheckHitKey(KEY_INPUT_UP) == 1)
	{
		dir.y = 1;
	}
	if (CheckHitKey(KEY_INPUT_DOWN) == 1)
	{
		dir.y = -1;
	}
	if (CheckHitKey(KEY_INPUT_LEFT) == 1)
	{
		dir.x = -1;
	}
	if (CheckHitKey(KEY_INPUT_RIGHT) == 1)
	{
		dir.x = 1;
	}

	// 余計な数値が入るので、純粋に長さ１の向きとして正規化する
	if (VSquareSize(dir) > 0)
	{
		dir = VNorm(dir);
	}

	// 向きにスピードをかけて、移動力に変更する
	rigidbody.SetVelocity(VScale(dir, speed));
}

/// <summary>
/// 描画
/// </summary>
void Player::Draw()
{
	// プレイヤーの顔＝1.0mのつもりで設定
	worldSprite->SetTransform(rigidbody.GetPos(), 1.0f);
	worldSprite->Draw();
	//DrawRotaGraph(
	//	static_cast<int>(rigidbody.GetPos().x),
	//	static_cast<int>(rigidbody.GetPos().y),
	//	1.0f,
	//	0,
	//	graphicHandle,
	//	TRUE);
}

/// <summary>
/// 衝突したとき
/// </summary>
void Player::OnCollide(const Collidable& colider)
{
	std::string message = "プレイヤーが";
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

