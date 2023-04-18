// 2023 Takeru Yui All Rights Reserved.
#include "Player.h"

/// <summary>
/// 初期化
/// </summary>
void Player::Init()
{
	shotIntervalCount = 0;

	// プレイヤーのグラフィックをメモリにロード＆表示座標を初期化
	Graph = LoadGraph("data/texture/EpicPlayer.png");
	X = SCREEN_W / 2;
	Y = SCREEN_H - 100;

	// プレイヤーと弾の画像のサイズを得る
	GetGraphSize(Graph, &W, &H);
}

/// <summary>
/// 更新
/// </summary>
/// <param name="shot">ショット配列</param>
void Player::Update(Shot shot[], int shotNum)
{
	// 矢印キーを押していたらプレイヤーを移動させる
	if (CheckHitKey(KEY_INPUT_UP) == 1)
	{
		Y -= PLAYER_SPEED;
	}
	if (CheckHitKey(KEY_INPUT_DOWN) == 1)
	{
		Y += PLAYER_SPEED;
	}
	if (CheckHitKey(KEY_INPUT_LEFT) == 1)
	{
		X -= PLAYER_SPEED;
	}
	if (CheckHitKey(KEY_INPUT_RIGHT) == 1)
	{
		X += PLAYER_SPEED;
	}

	// 弾の発射処理
	if (CheckHitKey(KEY_INPUT_SPACE) == 1)
	{
		// ショットを打つのを待つカウントがゼロになってたら弾を発射
		if (shotIntervalCount == 0)
		{
			// 画面上にでていない弾があるか、弾の数だけ繰り返して調べる
			for (int i = 0; i < shotNum; i++)
			{
				// 弾iが画面上にでていない場合はその弾を画面に出す
				if (shot[i].GetVisibleFlag() == false)
				{
					shot[i].OnShot(X, Y);

					// 一つ弾を出したので弾を出すループから抜けます
					break;
				}
			}

			// インターバル用のカウンターを設定.
			shotIntervalCount = SHOT_INTERVAL;
		}
	}

	if (shotIntervalCount != 0)
	{
		--shotIntervalCount;
	}

	// プレイヤーが画面左端からはみ出そうになっていたら画面内の座標に戻してあげる
	if (X < 0)
	{
		X = 0;
	}
	if (X > SCREEN_W - W)
	{
		X = SCREEN_W - W;
	}
	if (Y < 0)
	{
		Y = 0;
	}
	if (Y > SCREEN_H - H)
	{
		Y = SCREEN_H - H;
	}
}

/// <summary>
/// 描画
/// </summary>
void Player::Draw()
{
	DrawGraph(X, Y, Graph, TRUE);
}