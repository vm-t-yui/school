//-----------------------------------------------------------------------------
// @brief  プレイヤー処理.
//-----------------------------------------------------------------------------
#include "Common.h"
#include "DxLib.h"
#include "Shot.h"
#include "Player.h"

//----------------------------//
// プレイヤー関数群.
//----------------------------//
// コンストラクタ
Player::Player()
	: Graph(-1)
	, X(0)
	, Y(0)
	, W(0)
	, H(0)
	, shotIntervalCount(0)
{
	printfDx("コンストラクタ");
}

// デストラクタ
Player::~Player()
{
	printfDx("デストラクタ");
}

// 初期化.
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

// アップデート.
void Player::Update(ShotContainer& shotArray)
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
	//int shotArrayNum = static_cast<int>(shotArray.size());
	if (CheckHitKey(KEY_INPUT_SPACE) == 1)
	{
		// ショットを打つのを待つカウントがゼロになってたら弾を発射
		if (shotIntervalCount == 0)
		{
			// 画面上にでていない弾があるか、弾の数だけ繰り返して調べる
			for (auto& item : shotArray)
			{
				// 弾iが画面上にでていない場合はその弾を画面に出す
 				if (item->IsVisible() == false)
				{
					// 弾が発射されたとき
					item->OnShoot(X, Y, W, H);

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

// 描画.
void Player::Draw()
{
	DrawGraph(X, Y, Graph, TRUE);
}