#include "DxLib.h"
#include <vector>
#include "GlobalConstants.h"
#include "Player.h"
#include "Input.h"
#include "Shot.h"

/// <summary>
/// プレイヤーの初期化
/// </summary>
void Player::Initialize()
{
	// プレイヤーのグラフィックをメモリにロード＆表示座標を初期化
	pos = Player::FirstPos;
	dir = VGet(0, 0, 0);	// プレイヤーの向き
	graph = LoadGraph("data/texture/player.png");

	// キャラの画像の大きさを取得。毎度キャストするのがいやなので半分サイズも準備
	GetGraphSize(graph, &w, &h);
}

/// <summary>
/// プレイヤーの更新
/// </summary>
void Player::Update(const Input& input, std::vector<Shot>& shotArray)
{
	dir = VGet(0, 0, 0);	// 向きをリセット

	// 矢印キーを押していたらプレイヤーを移動させる
	if (CheckHitKey(KEY_INPUT_UP) == 1)
	{
		dir = VAdd(dir, VGet(0, -1, 0));
	}
	if (CheckHitKey(KEY_INPUT_DOWN) == 1)
	{
		dir = VAdd(dir, VGet(0, 1, 0));
	}
	if (CheckHitKey(KEY_INPUT_LEFT) == 1)
	{
		dir = VAdd(dir, VGet(-1, 0, 0));
	}
	if (CheckHitKey(KEY_INPUT_RIGHT) == 1)
	{
		dir = VAdd(dir, VGet(1, 0, 0));
	}

	// 長さがゼロじゃない場合、向きを正規化して、長さ1に
	if (VSize(dir) > 0)
	{
		dir = VNorm(dir);
	}

	// プレイヤーの移動
	VECTOR playerVelocity = VScale(dir, Player::Speed);	// 長さ1の向きに、大きさ（速度）をかける
	pos = VAdd(pos, playerVelocity);				// 座標ベクトルに、velicityを足すことで移動

	// 弾の発射処理
	// ボタンが押された瞬間だけ、発射処理を行う
	if (input.isPressedShotButton)
	{
		// 弾i個分繰り返す
		for (int i = 0; i < Shot::Num; i++)
		{
			// 弾が画面上にでていない場合はその弾を画面に出す
			if (shotArray[i].isAlive == false)
			{
				// 弾の発射位置をセット、プレイヤーの中心にする
				shotArray[i].pos = pos;

				// 弾が撃たれたので、存在状態を保持する変数にtrueを代入する
				shotArray[i].isAlive = true;

				break;	// 一発撃ったら抜ける
			}
		}
	}

	const float playerHalfW = w * 0.5f;
	const float playerHalfH = h * 0.5f;

	// プレイヤーが画面左端からはみ出そうになっていたら画面内の座標に戻してあげる
	if (pos.x < playerHalfW)
	{
		pos.x = playerHalfW;
	}
	if (pos.x > Graphics::ScreenW - playerHalfW)
	{
		pos.x = Graphics::ScreenW - playerHalfW;
	}
	if (pos.y < playerHalfH)
	{
		pos.y = playerHalfH;
	}
	if (pos.y > Graphics::ScreenH - playerHalfH)
	{
		pos.y = Graphics::ScreenH - playerHalfH;
	}
}

/// <summary>
/// プレイヤーを描画
/// </summary>
void Player::Draw() const
{
	const float playerHalfW = w * 0.5f;
	const float playerHalfH = h * 0.5f;

	DrawRotaGraph3(static_cast<int>(pos.x),
		static_cast<int>(pos.y),
		static_cast<int>(playerHalfW), static_cast<int>(playerHalfH),
		1.0f, 1.0f,
		0.0f, graph,
		FALSE, FALSE);
}