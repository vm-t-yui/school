// 2024 Takeru Yui All Rights Reserved.
#include "DxLib.h"
#include "Screen.h"
#include "YuiLib.h"
#include "Player.h"

/// <summary>
/// コンストラクタ
/// </summary>
Player::Player()
	: graphicHandle		(-1)
	, speed				(0)
{
	// 処理なし
}

/// <summary>
/// 初期化
/// </summary>
void Player::Initialize(YuiLib::Physics* physics)
{
	physics->Entry(this);	// 物理情報に自身を登録

	// プレイヤーのグラフィックをメモリにロード＆表示座標を初期化
	graphicHandle = LoadGraph("data/texture/player.png");

	// 物理挙動の初期化
	rigidbody.Initialize();
	rigidbody.SetPos(VGet(288, 400, 0));
	speed = 10;
}

/// <summary>
/// 終了
/// </summary>
void Player::Finalize(YuiLib::Physics* physics)
{
	physics->Exit(this);	// 物理情報登録解除
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
		dir.y = -1;
	}
	if (CheckHitKey(KEY_INPUT_DOWN) == 1)
	{
		dir.y = 1;
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
void Player::OnCollide()
{
	printfDx("プレイヤー当たった！");
}

