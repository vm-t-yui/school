// 2024 Takeru Yui All Rights Reserved.
#include "DxLib.h"
#include "Screen.h"
#include "YuiLib.h"
#include "Enemy.h"

/// <summary>
/// コンストラクタ
/// </summary>
Enemy::Enemy()
	: Collidable		(YuiLib::Collidable::Tag::Enemy)
	, graphicHandle		(-1)
{
	// 処理なし
}

/// <summary>
/// 初期化
/// </summary>
void Enemy::Initialize(YuiLib::Physics* physics)
{
	physics->Entry(this);	// 物理情報に自身を登録

	// 敵のグラフィックをメモリにロード＆表示座標を初期化
	graphicHandle = LoadGraph("data/texture/enemy.png");

	// 物理挙動の初期化
	rigidbody.Initialize();
	rigidbody.SetPos(VGet(200, 200, 0));
}

/// <summary>
/// 終了
/// </summary>
void Enemy::Finalize(YuiLib::Physics* physics)
{
	physics->Exit(this);	// 物理情報登録解除
}

/// <summary>
/// 更新
/// </summary>
void Enemy::Update()
{
	// TODO: 移動
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
void Enemy::OnCollide()
{
	// TODO:何と当たったかログに出す
	printfDx("敵が●●と当たった！");
}

