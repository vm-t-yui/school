//-----------------------------------------------------------------------------
// @brief  プレイヤークラス.
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#include "Player.h"

// 静的定数.
const float Player::ACCEL			= 0.03f;		// 通常の加速.
const float Player::DEFAULT_DECEL	= -0.03f;		// なにもしない時の減速.
const float Player::BREAK_DECEL		= -0.05f;		// ブレーキ時の減速.

//-----------------------------------------------------------------------------
// @brief  コンストラクタ.
//-----------------------------------------------------------------------------
Player::Player()
	: modelHandle(-1)
	 , accel(0.0f)
	 , speed(0.0f)
{
	// ３Ｄモデルの読み込み
	modelHandle = MV1LoadModel("data/model/player/hackadoll.pmx");

	pos = VGet(0, 0, 0);
	velocity = VGet(0, 0, 0);
}

//-----------------------------------------------------------------------------
// @brief  デストラクタ.
//-----------------------------------------------------------------------------
Player::~Player()
{
	// モデルのアンロード.
	MV1DeleteModel(modelHandle);
}

//-----------------------------------------------------------------------------
// @brief  更新.
//-----------------------------------------------------------------------------
void Player::Update()
{
	// キー入力取得
	int Key = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	// 上を押していたら上方向にディレクション補正＋加速.
	if (Key & PAD_INPUT_UP)
	{
		accel = ACCEL;
	}
	// 下を押していたら減速.
	if (Key & PAD_INPUT_DOWN)
	{
		accel = BREAK_DECEL;
	}
	// 右を押していたら右方向にディレクション補正.
	if (Key & PAD_INPUT_RIGHT)
	{
	}
	// 左を押していたら左方向にディレクション補正
	if (Key & PAD_INPUT_LEFT)
	{
	}
	// 何も押さなければ減速.
	if(Key == 0)
	{
		accel = DEFAULT_DECEL;
	}

	// 加速.
	speed += accel;
	if (speed < 0.0f)
	{
		speed = 0.0f;
	}

	// ベロシティ計算.
	velocity = VScale(VGet(0.0f, 0.0f, 1.0f), speed);

	// ポジションを更新.
	pos = VAdd(pos, velocity);

	// ３Dモデルのポジション設定
	MV1SetPosition(modelHandle, pos);
}

//-----------------------------------------------------------------------------
// @brief  描画.
//-----------------------------------------------------------------------------
void Player::Draw()
{
	// ３Ｄモデルの描画
	MV1DrawModel(modelHandle);
}