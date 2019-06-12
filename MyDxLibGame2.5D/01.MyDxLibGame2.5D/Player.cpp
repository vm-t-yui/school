//-----------------------------------------------------------------------------
// @brief  プレイヤークラス.
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#include "Player.h"

// 静的定数.
const float Player::SPEED				= 15.0f;		// 移動速度.

//-----------------------------------------------------------------------------
// @brief  コンストラクタ.
//-----------------------------------------------------------------------------
Player::Player()
	: graphicHandle(-1)
	, w(0)
	, h(0)
{
	// 画像の読み込み
	//const TCHAR* path = "data/texture/shimakaze.png";
	const TCHAR* path = "data/texture/player.png";
	graphicHandle = LoadGraph(path);
	if (graphicHandle <= 0)
	{
		printfDx("画像のロードに失敗:%s\n", path);
	}
	GetGraphSize(graphicHandle, &w, &h);

	pos = VGet(0, 0, 0);
	velocity = VGet(0, 0, 0);
	dir = VGet(0, 0, 1);
}

//-----------------------------------------------------------------------------
// @brief  デストラクタ.
//-----------------------------------------------------------------------------
Player::~Player()
{
	// 処理なし.
}

//-----------------------------------------------------------------------------
// @brief  更新.
//-----------------------------------------------------------------------------
void Player::Update()
{
	// キー入力取得
	int Key = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	// 上を押していたら.
	if (Key & PAD_INPUT_UP)
	{
		dir = VGet(0, 1, 0);
		velocity = VScale(dir, SPEED);
	}
	// 下を押していたら.
	else if (Key & PAD_INPUT_DOWN)
	{
		dir = VGet(0, -1, 0);
		velocity = VScale(dir, SPEED);
	}
	// 右を押していたら
	else if (Key & PAD_INPUT_RIGHT)
	{
		dir = VGet(1, 0, 0);
		velocity = VScale(dir, SPEED);
	}
	// 左を押していたら
	else if (Key & PAD_INPUT_LEFT)
	{
		dir = VGet(-1, 0, 0);
		velocity = VScale(dir, SPEED);
	}
	else
	{
		velocity = VGet(0, 0, 0);
	}

	// ポジションを更新.
	pos = VAdd(pos, velocity);

	// 画面外に出たら戻してやる.
	float screenLeft = -SCREEN_WIDTH_F * 0.5f;
	float screenRight = SCREEN_WIDTH_F * 0.5f;
	float screenUp = SCREEN_HEIGHT_F * 0.5f;
	float screenDown = -SCREEN_HEIGHT_F * 0.5f;
	if (pos.x < screenLeft + w * 0.5f)
	{
		pos.x = screenLeft + w * 0.5f;
	}
	else if (pos.x > screenRight - w * 0.5f)
	{
		pos.x = screenRight - w * 0.5f;
	}
	if (pos.y < screenDown + h * 0.5f)
	{
		pos.y = screenDown + h * 0.5f;
	}
	else if (pos.y > screenUp - h * 0.5f)
	{
		pos.y = screenUp - h * 0.5f;
	}
}

//-----------------------------------------------------------------------------
// @brief  描画.
//-----------------------------------------------------------------------------
void Player::Draw()
{
	DrawBillboard3D(pos, 0.5f, 0.5f, GRAPHIC_SCALE_FACTOR_2POINT5D, 0, graphicHandle, TRUE);
}
