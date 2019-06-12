#include "common.h"
#include "player.h"
#include "DxLib.h"

//----------------------------//
// プレイヤー関数群.
//----------------------------//
// 初期化.
void InitPlayer(Player& player)
{
	// プレイヤーのグラフィックをメモリにロード＆表示座標を初期化
	player.Graph = LoadGraph("data/texture/EpicPlayer.png");
	player.X = SCREEN_W / 2;
	player.Y = SCREEN_H - 100;

	// プレイヤーと弾の画像のサイズを得る
	GetGraphSize(player.Graph, &player.W, &player.H);

	player.ShotIntervalCount = 0;
}

// アップデート.
void UpdatePlayer(Player& player, Shot* shot)
{
	// 矢印キーを押していたらプレイヤーを移動させる
	if (CheckHitKey(KEY_INPUT_UP) == 1)
	{
		player.Y -= PLAYER_SPEED;
	}
	if (CheckHitKey(KEY_INPUT_DOWN) == 1)
	{
		player.Y += PLAYER_SPEED;
	}
	if (CheckHitKey(KEY_INPUT_LEFT) == 1)
	{
		player.X -= PLAYER_SPEED;
	}
	if (CheckHitKey(KEY_INPUT_RIGHT) == 1)
	{
		player.X += PLAYER_SPEED;
	}

	// 弾の発射処理
	if (CheckHitKey(KEY_INPUT_SPACE) == 1)
	{
		// 前フレームでショットボタンを押したかが保存されている変数がfalseだったら弾を発射
		if (player.ShotIntervalCount == 0)
		{
			// 画面上にでていない弾があるか、弾の数だけ繰り返して調べる
			for (int i = 0; i < SHOT; i++)
			{
				// 弾iが画面上にでていない場合はその弾を画面に出す
				if (shot[i].VisibleFlag == false)
				{
					// 弾iの位置をセット、位置はプレイヤーの中心にする
					shot[i].X = (player.W - shot[i].W) / 2 + player.X;
					shot[i].Y = (player.H - shot[i].H) / 2 + player.Y;

					// 弾iは現時点を持って存在するので、存在状態を保持する変数にtrueを代入する
					shot[i].VisibleFlag = true;

					// 一つ弾を出したので弾を出すループから抜けます
					break;
				}
			}

			// インターバル用のカウンターを設定.
			player.ShotIntervalCount = SHOT_INTERVAL;
		}
	}

	if (player.ShotIntervalCount != 0)
	{
		--player.ShotIntervalCount;
	}

	// プレイヤーが画面左端からはみ出そうになっていたら画面内の座標に戻してあげる
	if (player.X < 0)
	{
		player.X = 0;
	}
	if (player.X > SCREEN_W - player.W)
	{
		player.X = SCREEN_W - player.W;
	}
	if (player.Y < 0)
	{
		player.Y = 0;
	}
	if (player.Y > SCREEN_H - player.H)
	{
		player.Y = SCREEN_H - player.H;
	}
}

// 描画.
void DrawPlayer(Player& player)
{
	DrawGraph(player.X, player.Y, player.Graph, TRUE);
}