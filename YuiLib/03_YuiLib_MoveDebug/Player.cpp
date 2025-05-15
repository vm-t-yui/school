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

	// 初期化
	pos = VGet(288, 400, 0);
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
	auto velocity = VScale(dir, speed);
	
	// ポジションに移動力を足す.
	pos = VAdd(pos, velocity);

	// プレイヤーが画面端からはみ出そうになっていたら画面内の座標に戻してあげる
	int w = 64, h = 64;
	if (pos.x < 0 + (w * 0.5f))
	{
		pos.x = 0 + (w * 0.5f);
	}
	if (pos.x > SCREEN_WIDTH - (w * 0.5f))
	{
		pos.x = static_cast<float>(SCREEN_WIDTH - (w * 0.5f));
	}
	if (pos.y < 0 + (h * 0.5f))
	{
		pos.y = 0 + (h * 0.5f);
	}
	if (pos.y > static_cast<float>(SCREEN_HEIGHT - (h * 0.5f)))
	{
		pos.y = static_cast<float>(SCREEN_HEIGHT - (h * 0.5f));
	}
}

/// <summary>
/// 描画
/// </summary>
void Player::Draw()
{
	DrawRotaGraph(
		static_cast<int>(pos.x),
		static_cast<int>(pos.y),
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

