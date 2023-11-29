#include "DxLib.h"
#include "Camera.h"
#include "Input.h"
#include "Player.h"
#include "Stage.h"

/// <summary>
/// コンストラクタ
/// </summary>
Camera::Camera()
	:  angleH	(0)
	 , angleV	(0)
	 , position	(VGet(0, 0, 0))	
	 , target	(VGet(0, 0, 0))
{
	// カメラの初期水平角度は１８０度
	angleH = DX_PI_F;

	// 垂直角度は０度
	angleV = 0.0f;
}

/// <summary>
/// デストラクタ
/// </summary>
Camera::~Camera()
{
	// 処理なし
}

/// <summary>
/// 更新
/// </summary>
void Camera::Update(const Input& input, const Player& player, const Stage& stage)
{
	// パッドの３ボタンか、シフトキーが押されている場合のみ角度変更操作を行う
	if (CheckHitKey(KEY_INPUT_LSHIFT) || (input.GetNowFrameInput() & PAD_INPUT_C))
	{
		// 「←」ボタンが押されていたら水平角度をマイナスする
		if (input.GetNowFrameInput() & PAD_INPUT_LEFT)
		{
			angleH -= AngleSpeed;

			// －１８０度以下になったら角度値が大きくなりすぎないように３６０度を足す
			if (angleH < -DX_PI_F)
			{
				angleH += DX_TWO_PI_F;
			}
		}

		// 「→」ボタンが押されていたら水平角度をプラスする
		if (input.GetNowFrameInput() & PAD_INPUT_RIGHT)
		{
			angleH += AngleSpeed;

			// １８０度以上になったら角度値が大きくなりすぎないように３６０度を引く
			if (angleH > DX_PI_F)
			{
				angleH -= DX_TWO_PI_F;
			}
		}

		// 「↑」ボタンが押されていたら垂直角度をマイナスする
		if (input.GetNowFrameInput() & PAD_INPUT_UP)
		{
			angleV -= AngleSpeed;

			// ある一定角度以下にはならないようにする
			if (angleV < -DX_PI_F * 0.5f + 0.6f)
			{
				angleV = -DX_PI_F * 0.5f + 0.6f;
			}
		}

		// 「↓」ボタンが押されていたら垂直角度をプラスする
		if (input.GetNowFrameInput() & PAD_INPUT_DOWN)
		{
			angleV += AngleSpeed;

			// ある一定角度以上にはならないようにする
			if (angleV > DX_PI_F * 0.5f - 0.6f)
			{
				angleV = DX_PI_F * 0.5f - 0.6f;
			}
		}
	}

	// カメラの注視点はプレイヤー座標から規定値分高い座標
	target = VAdd(player.GetPosition(), VGet(0.0f, CameraPlayerTargetHeight, 0.0f));

	// カメラの座標を補正する
	FixCameraPosition(stage);

	// カメラの情報をライブラリのカメラに反映させる
	SetCameraPositionAndTarget_UpVecY(position, target);
}

/// <summary>
/// カメラ座標を補正する
/// </summary>
void Camera::FixCameraPosition(const Stage& stage)
{
	// 水平方向の回転はＹ軸回転
	auto rotY = MGetRotY(angleH);

	// 垂直方向の回転はＺ軸回転 )
	auto rotZ = MGetRotZ(angleV);

	// カメラからプレイヤーまでの初期距離をセット
	float cameraPlayerLength = ToPlayerLength;

	// カメラの座標を算出
	// Ｘ軸にカメラとプレイヤーとの距離分だけ伸びたベクトルを
	// 垂直方向回転( Ｚ軸回転 )させたあと水平方向回転( Ｙ軸回転 )して更に
	// 注視点の座標を足したものがカメラの座標
	position = VAdd(VTransform(VTransform(VGet(-cameraPlayerLength, 0.0f, 0.0f), rotZ), rotY), target);

	// あたらない位置までプレイヤーに近づく
	// ポリゴンに当たらない距離をセット
	float notHitLength = 0.0f;

	// ポリゴンに当たる距離をセット
	float hitLength = cameraPlayerLength;

	// 注視点からカメラの座標までの間にステージのポリゴンがあるか調べる
	do
	{
		// 当たるかどうかテストする距離をセット( 当たらない距離と当たる距離の中間 )
		float testLength = notHitLength + (hitLength - notHitLength) * 0.5f;

		// テスト用のカメラ座標を算出
		auto testPosition = VAdd(VTransform(VTransform(VGet(-testLength, 0.0f, 0.0f), rotZ), rotY), target);

		// 新しい座標で壁に当たるかテスト
		auto hitResult = MV1CollCheck_Capsule(stage.GetModelHandle(), -1, target, testPosition, CollisionSize);
		int hitNum = hitResult.HitNum;
		MV1CollResultPolyDimTerminate(hitResult);
		if (hitNum != 0)
		{
			// 当たったら当たる距離を testLength に変更する
			hitLength = testLength;

			// カメラの座標をセット
			position = testPosition;
		}
		else
		{
			// 当たらなかったら当たらない距離を testLength に変更する
			notHitLength = testLength;
		}

		// hitLength と NoHitLength が十分に近づいていなかったらループ
	} while (hitLength - notHitLength > 0.1f);
}