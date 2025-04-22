//-----------------------------------------------------------------------------
// @brief  プレイヤークラス.
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#include "Player.h"

// 静的定数.
const float Player::ACCEL				= 0.03f;		// 通常の加速.
const float Player::DEFAULT_DECEL		= -0.01f;		// なにもしない時の減速.
const float Player::BREAK_DECEL			= -0.05f;		// ブレーキ時の減速.
const float Player::GRIP_DECEL			= -0.025f;		// グリップの減速.
const float Player::GRIP_POWER			= 0.1f;			// グリップの力.

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
	dir = VGet(0, 0, 1);
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

	// 上を押していたら加速.
	if (Key & PAD_INPUT_UP)
	{
		accel = ACCEL;
	}
	// 下を押していたら減速.
	if (Key & PAD_INPUT_DOWN)
	{
		accel = BREAK_DECEL;
	}
	// 右か左を押していたらグリップによる減速.
	if (Key & PAD_INPUT_RIGHT || Key & PAD_INPUT_LEFT)
	{
		accel += GRIP_DECEL;
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
	velocity = VScale(dir, speed);

	// 右を押していたら右方向に力をかける
	if (Key & PAD_INPUT_RIGHT)
	{
		VECTOR right = VCross(VGet(0.0f, 1.0f, 0.0f), dir);
		velocity = VAdd(velocity, VScale(right, GRIP_POWER));
	}
	// 左を押していたら左方向に力をかける
	if (Key & PAD_INPUT_LEFT)
	{
		VECTOR left = VCross(dir, VGet(0.0f, 1.0f, 0.0f));
		velocity = VAdd(velocity, VScale(left, GRIP_POWER));
	}

	// ポジションを更新.
	pos = VAdd(pos, velocity);

	// 力をかけ終わったベロシティの方向にディレクションを調整.
	if (VSize(velocity) != 0)
	{
		dir = VNorm(velocity);
	}
	//printfDx("%f %f %f\n", dir.x, dir.y, dir.z);

	// ３Dモデルのポジション設定
	MV1SetPosition(modelHandle, pos);

	// 向きに合わせて回転.
	MV1SetRotationZYAxis(modelHandle, dir, VGet(0.0f, 1.0f, 0.0f), 0.0f);
	
	// モデルに向いてほしい方向に回転.
	MATRIX tmpMat = MV1GetMatrix(modelHandle);
	MATRIX rotYMat = MGetRotY(180.0f * (float)(DX_PI / 180.0f));
	tmpMat = MMult(tmpMat, rotYMat);
	MV1SetRotationMatrix(modelHandle, tmpMat);
}

//-----------------------------------------------------------------------------
// @brief  描画.
//-----------------------------------------------------------------------------
void Player::Draw()
{
	// ３Ｄモデルの描画
	MV1DrawModel(modelHandle);
}