#pragma once

// プレイヤー構造体.
struct Player
{
	int Graph;
	int X;
	int Y;
	int W;
	int H;
	bool PrevShotFlag;
};

void InitializePlayer(Player& player);									// プレイヤー初期化
void UpdatePlayer(Player& player, Shot shotArray[], int shotArraySize);	// プレイヤーの更新処理
void DrawPlayer(Player& player);										// プレイヤーの描画処理

// プレイヤー初期化
void InitializePlayer(Player& player)
{
	// プレイヤーのグラフィックをメモリにロード＆表示座標を初期化
	player.Graph = LoadGraph("data/texture/player.png");
	player.X = 288;
	player.Y = 400;

	// ショットボタンが前のフレームで押されたかどうかを保存する変数にfalse(押されいない)を代入
	player.PrevShotFlag = false;

	// プレイヤーと弾の画像のサイズを得る
	GetGraphSize(player.Graph, &player.W, &player.H);
}

// プレイヤーの更新処理
void UpdatePlayer(Player& player, Shot shotArray[], int shotArraySize)
{
	// 矢印キーを押していたらプレイヤーを移動させる
	if (CheckHitKey(KEY_INPUT_UP) == 1)
	{
		player.Y -= 3;
	}
	if (CheckHitKey(KEY_INPUT_DOWN) == 1)
	{
		player.Y += 3;
	}
	if (CheckHitKey(KEY_INPUT_LEFT) == 1)
	{
		player.X -= 3;
	}
	if (CheckHitKey(KEY_INPUT_RIGHT) == 1)
	{
		player.X += 3;
	}

	// 弾の発射処理
	if (CheckHitKey(KEY_INPUT_SPACE))
	{
		// 前フレームでショットボタンを押したかが保存されている変数がfalseだったら弾を発射
		if (player.PrevShotFlag == false)
		{
			// 画面上にでていない弾があるか、弾の数だけ繰り返して調べる
			for (int i = 0; i < shotArraySize; i++)
			{
				// 弾iが画面上にでていない場合はその弾を画面に出す
				if (shotArray[i].visibleFlag == false)
				{
					// 弾iの位置をセット、位置はプレイヤーの中心にする
					shotArray[i].X = (player.W - shotArray[i].W) / 2 + player.X;
					shotArray[i].Y = (player.H - shotArray[i].H) / 2 + player.Y;

					// 弾iは現時点を持って存在するので、存在状態を保持する変数にtrueを代入する
					shotArray[i].visibleFlag = true;

					// 一つ弾を出したので弾を出すループから抜けます
					break;
				}
			}
		}

		// 前フレームでショットボタンを押されていたかを保存する変数にtrue(おされていた)を代入
		player.PrevShotFlag = true;
	}
	else
	{
		// ショットボタンが押されていなかった場合は
		// 前フレームでショットボタンが押されていたかを保存する変数にfalse(おされていない)を代入
		player.PrevShotFlag = false;
	}

	// プレイヤーが画面左端からはみ出そうになっていたら画面内の座標に戻してあげる
	if (player.X < 0)
	{
		player.X = 0;
	}
	if (player.X > 640 - player.W)
	{
		player.X = 640 - player.W;
	}
	if (player.Y < 0)
	{
		player.Y = 0;
	}
	if (player.Y > 480 - player.H)
	{
		player.Y = 480 - player.H;
	}
}

// プレイヤーの描画処理
void DrawPlayer(Player& player)
{
	// プレイヤーを描画
	DrawGraph(player.X, player.Y, player.Graph, FALSE);
}
