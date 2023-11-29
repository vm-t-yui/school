#include <math.h>
#include "DxLib.h"
#include "Camera.h"
#include "Input.h"
#include "Stage.h"
#include "Player.h"

/// <summary>
/// コンストラクタ
/// </summary>
Player::Player()
	:  position				(VGet(0.0f, 0.0f, 0.0f))
	 , targetMoveDirection	(VGet(1.0f, 0.0f, 0.0f))
	 , angle				(0.0f)
	 , currentJumpPower		(0.0f)
	 , modelHandle			(-1)
	 , shadowHandle			(-1)
	 , currentState			(State::Stand)
	 , currentPlayAnim		(-1)
	 , currentAnimCount		(0)
	 , prevPlayAnim			(-1)
	 , prevAnimCount		(0)
	 , animBlendRate		(1.0f)
	 , isMove				(false)
{
	// 処理なし
}

/// <summary>
/// デストラクタ
/// </summary>
Player::~Player()
{
	Unload();
}

/// <summary>
/// 初期化
/// </summary>
void Player::Load()
{
	// モデルの読み込み
	modelHandle = MV1LoadModel("DxChara.x");

	// 影描画用の画像の読み込み
	shadowHandle = LoadGraph("Shadow.tga");

	// アニメーションのブレンド率を初期化
	animBlendRate = 1.0f;

	// 初期状態ではアニメーションはアタッチされていないにする
	currentPlayAnim = -1;
	prevPlayAnim = -1;

	// ただ立っているアニメーションを再生
	PlayAnim(AnimKind::Stop);
}

/// <summary>
/// アンロード
/// </summary>
void Player::Unload()
{
	// モデルの削除
	if (shadowHandle >= 0)
	{
		MV1DeleteModel(modelHandle);
		modelHandle = -1;
	}
	// 影用画像の削除
	if (shadowHandle >= 0)
	{
		DeleteGraph(shadowHandle);
		shadowHandle = -1;
	}
}

/// <summary>
/// 更新
/// </summary>
void Player::Update(const Input& input, const Camera& camera, Stage& stage)
{
	// ルートフレームのＺ軸方向の移動パラメータを無効にする
	DisableRootFrameZMove();

	// パッド入力によって移動パラメータを設定する
	VECTOR	upMoveVec;		// 方向ボタン「↑」を入力をしたときのプレイヤーの移動方向ベクトル
	VECTOR	leftMoveVec;	// 方向ボタン「←」を入力をしたときのプレイヤーの移動方向ベクトル
	VECTOR	moveVec;		// このフレームの移動ベクトル
	State prevState = currentState;
	currentState = UpdateMoveParameterWithPad(input, camera, upMoveVec, leftMoveVec, moveVec);

	// アニメーションステートの更新
	UpdateAnimationState(prevState);

	// プレイヤーの移動方向にモデルの方向を近づける
	UpdateAngle();

	// 移動ベクトルを元にコリジョンを考慮しつつプレイヤーを移動
	Move(moveVec, stage);

	// アニメーション処理
	UpdateAnimation();
}

/// <summary>
/// 描画
/// </summary>
void Player::Draw(const Stage& stage)
{
	MV1DrawModel(modelHandle);
	DrawShadow(stage);
}

/// <summary>
/// ルートフレームのＺ軸方向の移動パラメータを無効にする
/// </summary>
void Player::DisableRootFrameZMove()
{
	// HACK:
	// ・DXライブラリのモデルファイル内には、複数のメッシュ（ポリゴンの集合）やカメラ、ライトを入れることができる
	// ・置いた複数のファイルは、親子関係をつけたり、Unityのヒエラルキーみたいに、階層構造が作れる
	// ・この階層それぞれには名前が付けられる Root-Meshes-Model1
	//											         |-Model2
	// ・この名前の付いた階層のことをDXライブラリではフレームという
	// ・一番親の階層を「ルートフレーム」と呼ぶ。ルートフレームは一つ
	// 
	// HACK: 何のために？モデルの一番親フレーム（親階層）のZ軸方向の移動パラメータをゼロにしている

	MATRIX localMatrix;

	// ユーザー行列を解除する
	MV1ResetFrameUserLocalMatrix(modelHandle, 2);

	// 現在のルートフレームの行列を取得する
	localMatrix = MV1GetFrameLocalMatrix(modelHandle, 2);

	// Ｚ軸方向の平行移動成分を無効にする
	localMatrix.m[3][2] = 0.0f;

	// ユーザー行列として平行移動成分を無効にした行列をルートフレームにセットする
	MV1SetFrameUserLocalMatrix(modelHandle, 2, localMatrix);
}

/// <summary>
/// パッド入力によって移動パラメータを設定する
/// </summary>
Player::State Player::UpdateMoveParameterWithPad(const Input& input, const Camera& camera, VECTOR& upMoveVec, VECTOR& leftMoveVec, VECTOR& moveVec)
{
	State nextState = currentState;

	// プレイヤーの移動方向のベクトルを算出
	// 方向ボタン「↑」を押したときのプレイヤーの移動ベクトルはカメラの視線方向からＹ成分を抜いたもの
	upMoveVec = VSub(camera.GetTarget(), camera.GetPosition());
	upMoveVec.y = 0.0f;

	// 方向ボタン「←」を押したときのプレイヤーの移動ベクトルは上を押したときの方向ベクトルとＹ軸のプラス方向のベクトルに垂直な方向
	leftMoveVec = VCross(upMoveVec, VGet(0.0f, 1.0f, 0.0f));

	// 二つのベクトルを正規化( ベクトルの長さを１．０にすること )
	upMoveVec = VNorm(upMoveVec);
	leftMoveVec = VNorm(leftMoveVec);

	// このフレームでの移動ベクトルを初期化
	moveVec = VGet(0.0f, 0.0f, 0.0f);

	// 移動したかどうかのフラグを初期状態では「移動していない」を表すFALSEにする
	bool isPressMoveButton = false;

	// パッドの３ボタンと左シフトがどちらも押されていなかったらプレイヤーの移動処理
	if (CheckHitKey(KEY_INPUT_LSHIFT) == 0 && (input.GetNowFrameInput() & PAD_INPUT_C) == 0)
	{
		// 方向ボタン「←」が入力されたらカメラの見ている方向から見て左方向に移動する
		if (input.GetNowFrameInput() & PAD_INPUT_LEFT)
		{
			// 移動ベクトルに「←」が入力された時の移動ベクトルを加算する
			moveVec = VAdd(moveVec, leftMoveVec);

			// 移動したかどうかのフラグを「移動した」にする
			isPressMoveButton = true;
		}
		else
			// 方向ボタン「→」が入力されたらカメラの見ている方向から見て右方向に移動する
			if (input.GetNowFrameInput() & PAD_INPUT_RIGHT)
			{
				// 移動ベクトルに「←」が入力された時の移動ベクトルを反転したものを加算する
				moveVec = VAdd(moveVec, VScale(leftMoveVec, -1.0f));

				// 移動したかどうかのフラグを「移動した」にする
				isPressMoveButton = true;
			}

		// 方向ボタン「↑」が入力されたらカメラの見ている方向に移動する
		if (input.GetNowFrameInput() & PAD_INPUT_UP)
		{
			// 移動ベクトルに「↑」が入力された時の移動ベクトルを加算する
			moveVec = VAdd(moveVec, upMoveVec);

			// 移動したかどうかのフラグを「移動した」にする
			isPressMoveButton = true;
		}
		else
			// 方向ボタン「↓」が入力されたらカメラの方向に移動する
			if (input.GetNowFrameInput() & PAD_INPUT_DOWN)
			{
				// 移動ベクトルに「↑」が入力された時の移動ベクトルを反転したものを加算する
				moveVec = VAdd(moveVec, VScale(upMoveVec, -1.0f));

				// 移動したかどうかのフラグを「移動した」にする
				isPressMoveButton = true;
			}

		// プレイヤーの状態が「ジャンプ」ではなく、且つボタン１が押されていたらジャンプする
		if (currentState != State::Jump && (input.GetNowFrameNewInput() & PAD_INPUT_A))
		{
			// Ｙ軸方向の速度をセット
			currentJumpPower = JumpPower;

			// 状態を「ジャンプ」にする
			nextState = State::Jump;
		}
	}

	// ジャンプ状態なら重力適用
	if (currentState == State::Jump)
	{
		// Ｙ軸方向の速度を重力分減算する
		currentJumpPower -= Gravity;
	}

	// 移動ボタンが押されたかどうかで処理を分岐
	if (isPressMoveButton)
	{
		// もし今まで「立ち止まり」状態だったら
		if (currentState == State::Stand)
		{
			// 状態を「走り」にする
			nextState = State::Run;
		}

		// 移動ベクトルを正規化したものをプレイヤーが向くべき方向として保存
		targetMoveDirection = VNorm(moveVec);

		// プレイヤーが向くべき方向ベクトルをプレイヤーのスピード倍したものを移動ベクトルとする
		moveVec = VScale(targetMoveDirection, MoveSpeed);
	}
	else
	{
		// このフレームで移動していなくて、且つ状態が「走り」だったら
		if (currentState == State::Run)
		{
			// 状態を「立ち止り」にする
			nextState = State::Stand;
		}
	}

	// 移動ベクトルのＹ成分をＹ軸方向の速度にする
	moveVec.y = currentJumpPower;

	return nextState;
}

/// <summary>
/// 移動処理
/// </summary>
void Player::Move(const VECTOR& MoveVector, Stage& stage)
{
	// HACK: 移動距離が0.01未満で微妙に移動していた場合はじんわり移動してバグる
	// x軸かy軸方向に 0.01f 以上移動した場合は「移動した」フラグを１にする
	if (fabs(MoveVector.x) > 0.01f || fabs(MoveVector.z) > 0.01f)
	{
		isMove = true;
	}
	else
	{
		isMove = false;
	}

	// 当たり判定をして、新しい座標を保存する
	position = stage.CheckCollision(*this, MoveVector);

	// プレイヤーのモデルの座標を更新する
	MV1SetPosition(modelHandle, position);
}

/// <summary>
/// アニメーションステートの更新
/// </summary>
void Player::UpdateAnimationState(State prevState)
{
	// 立ち止まりから走りに変わったら
	if (prevState == State::Stand && currentState == State::Run)
	{
		// 走りアニメーションを再生する
		PlayAnim(AnimKind::Run);
	}
	// 走りから立ち止まりに変わったら
	else if (prevState == State::Run && currentState == State::Stand)
	{
		// 立ち止りアニメーションを再生する
		PlayAnim(AnimKind::Stop);
	}
	// 状態が「ジャンプ」の場合は
	else if (currentState == State::Jump)
	{
		// もし落下していて且つ再生されているアニメーションが上昇中用のものだった場合は
		if (currentJumpPower < 0.0f)
		{
			// 落下中ようのアニメーションを再生する
			if (MV1GetAttachAnim(modelHandle, currentPlayAnim) == static_cast<int>(AnimKind::Jump))
			{
				PlayAnim(AnimKind::Fall);
			}
		}
		else
		{
			PlayAnim(AnimKind::Jump);
		}
	}
}

/// <summary>
/// プレイヤーの回転制御
/// </summary>
void Player::UpdateAngle()
{
	// プレイヤーの移動方向にモデルの方向を近づける
	float targetAngle;			// 目標角度
	float difference;			// 目標角度と現在の角度との差

	// 目標の方向ベクトルから角度値を算出する
	targetAngle = static_cast<float>(atan2(targetMoveDirection.x, targetMoveDirection.z));

	// 目標の角度と現在の角度との差を割り出す
	// 最初は単純に引き算
	difference = targetAngle - angle;

	// ある方向からある方向の差が１８０度以上になることは無いので
	// 差の値が１８０度以上になっていたら修正する
	if (difference < -DX_PI_F)
	{
		difference += DX_TWO_PI_F;
	}
	else if (difference > DX_PI_F)
	{
		difference -= DX_TWO_PI_F;
	}

	// 角度の差が０に近づける
	if (difference > 0.0f)
	{
		// 差がプラスの場合は引く
		difference -= AngleSpeed;
		if (difference < 0.0f)
		{
			difference = 0.0f;
		}
	}
	else
	{
		// 差がマイナスの場合は足す
		difference += AngleSpeed;
		if (difference > 0.0f)
		{
			difference = 0.0f;
		}
	}

	// モデルの角度を更新
	angle = targetAngle - difference;
	MV1SetRotationXYZ(modelHandle, VGet(0.0f, angle + DX_PI_F, 0.0f));
}

/// <summary>
/// プレイヤーのアニメーションを再生する
/// </summary>
void Player::PlayAnim(AnimKind nextPlayAnim)
{
	// HACK: 指定した番号のアニメーションをアタッチし、直前に再生していたアニメーションの情報をprevに移行している
	// 入れ替えを行うので、１つ前のモーションがが有効だったらデタッチする
	if (prevPlayAnim != -1)
	{
		MV1DetachAnim(modelHandle, prevPlayAnim);
		prevPlayAnim = -1;
	}

	// 今まで再生中のモーションだったものの情報をPrevに移動する
	prevPlayAnim = currentPlayAnim;
	prevAnimCount = currentAnimCount;

	// 新たに指定のモーションをモデルにアタッチして、アタッチ番号を保存する
	currentPlayAnim = MV1AttachAnim(modelHandle, static_cast<int>(nextPlayAnim));
	currentAnimCount = 0.0f;

	// ブレンド率はPrevが有効ではない場合は１．０ｆ( 現在モーションが１００％の状態 )にする
	animBlendRate = prevPlayAnim == -1 ? 1.0f : 0.0f;
}

/// <summary>
/// プレイヤーのアニメーション処理
/// </summary>
void Player::UpdateAnimation()
{
	float animTotalTime;		// 再生しているアニメーションの総時間

	// ブレンド率が１以下の場合は１に近づける
	if (animBlendRate < 1.0f)
	{
		animBlendRate += AnimBlendSpeed;
		if (animBlendRate > 1.0f)
		{
			animBlendRate = 1.0f;
		}
	}

	// 再生しているアニメーション１の処理
	if (currentPlayAnim != -1)
	{
		// アニメーションの総時間を取得
		animTotalTime = MV1GetAttachAnimTotalTime(modelHandle, currentPlayAnim);

		// 再生時間を進める
		currentAnimCount += PlayAnimSpeed;

		// 再生時間が総時間に到達していたら再生時間をループさせる
		if (currentAnimCount >= animTotalTime)
		{
			currentAnimCount = static_cast<float>(fmod(currentAnimCount, animTotalTime));
		}

		// 変更した再生時間をモデルに反映させる
		MV1SetAttachAnimTime(modelHandle, currentPlayAnim, currentAnimCount);

		// アニメーション１のモデルに対する反映率をセット
		MV1SetAttachAnimBlendRate(modelHandle, currentPlayAnim, animBlendRate);
	}

	// 再生しているアニメーション２の処理
	if (prevPlayAnim != -1)
	{
		// アニメーションの総時間を取得
		animTotalTime = MV1GetAttachAnimTotalTime(modelHandle, prevPlayAnim);

		// 再生時間を進める
		prevAnimCount += PlayAnimSpeed;

		// 再生時間が総時間に到達していたら再生時間をループさせる
		if (prevAnimCount > animTotalTime)
		{
			prevAnimCount = static_cast<float>(fmod(prevAnimCount, animTotalTime));
		}

		// 変更した再生時間をモデルに反映させる
		MV1SetAttachAnimTime(modelHandle, prevPlayAnim, prevAnimCount);

		// アニメーション２のモデルに対する反映率をセット
		MV1SetAttachAnimBlendRate(modelHandle, prevPlayAnim, 1.0f - animBlendRate);
	}
}

/// <summary>
/// プレイヤーの影を描画
/// </summary>
void Player::DrawShadow(const Stage& stage)
{
	// ライティングを無効にする
	SetUseLighting(FALSE);

	// Ｚバッファを有効にする
	SetUseZBuffer3D(TRUE);

	// テクスチャアドレスモードを CLAMP にする( テクスチャの端より先は端のドットが延々続く )
	SetTextureAddressMode(DX_TEXADDRESS_CLAMP);

	// プレイヤーの直下に存在する地面のポリゴンを取得
	auto hitResultDim = MV1CollCheck_Capsule(stage.GetModelHandle(), -1, position, VAdd(position, VGet(0.0f, -ShadowHeight, 0.0f)), ShadowSize);

	// 頂点データで変化が無い部分をセット
	VERTEX3D vertex[3];
	vertex[0].dif = GetColorU8(255, 255, 255, 255);
	vertex[0].spc = GetColorU8(0, 0, 0, 0);
	vertex[0].su = 0.0f;
	vertex[0].sv = 0.0f;
	vertex[1] = vertex[0];
	vertex[2] = vertex[0];

	// 球の直下に存在するポリゴンの数だけ繰り返し
	auto hitResult = hitResultDim.Dim;
	for (int i = 0; i < hitResultDim.HitNum; i++, hitResult++)
	{
		// ポリゴンの座標は地面ポリゴンの座標
		vertex[0].pos = hitResult->Position[0];
		vertex[1].pos = hitResult->Position[1];
		vertex[2].pos = hitResult->Position[2];

		// ちょっと持ち上げて重ならないようにする
		auto slideVec = VScale(hitResult->Normal, 0.5f);
		vertex[0].pos = VAdd(vertex[0].pos, slideVec);
		vertex[1].pos = VAdd(vertex[1].pos, slideVec);
		vertex[2].pos = VAdd(vertex[2].pos, slideVec);

		// ポリゴンの不透明度を設定する
		vertex[0].dif.a = 0;
		vertex[1].dif.a = 0;
		vertex[2].dif.a = 0;
		if (hitResult->Position[0].y > position.y - ShadowHeight)
			vertex[0].dif.a = static_cast<BYTE>(128 * (1.0f - static_cast<float>(fabs(hitResult->Position[0].y - position.y) / ShadowHeight)));

		if (hitResult->Position[1].y > position.y - ShadowHeight)
			vertex[1].dif.a = static_cast<BYTE>(128 * (1.0f - static_cast<float>(fabs(hitResult->Position[1].y - position.y) / ShadowHeight)));

		if (hitResult->Position[2].y > position.y - ShadowHeight)
			vertex[2].dif.a = static_cast<BYTE>(128 * (1.0f - static_cast<float>(fabs(hitResult->Position[2].y - position.y) / ShadowHeight)));

		// ＵＶ値は地面ポリゴンとプレイヤーの相対座標から割り出す
		vertex[0].u = (hitResult->Position[0].x - position.x) / (ShadowSize * 2.0f) + 0.5f;
		vertex[0].v = (hitResult->Position[0].z - position.z) / (ShadowSize * 2.0f) + 0.5f;
		vertex[1].u = (hitResult->Position[1].x - position.x) / (ShadowSize * 2.0f) + 0.5f;
		vertex[1].v = (hitResult->Position[1].z - position.z) / (ShadowSize * 2.0f) + 0.5f;
		vertex[2].u = (hitResult->Position[2].x - position.x) / (ShadowSize * 2.0f) + 0.5f;
		vertex[2].v = (hitResult->Position[2].z - position.z) / (ShadowSize * 2.0f) + 0.5f;

		// 影ポリゴンを描画
		DrawPolygon3D(vertex, 1, shadowHandle, TRUE);
	}

	// 検出した地面ポリゴン情報の後始末
	MV1CollResultPolyDimTerminate(hitResultDim);

	// ライティングを有効にする
	SetUseLighting(TRUE);

	// Ｚバッファを無効にする
	SetUseZBuffer3D(FALSE);
}

/// <summary>
/// 天井に当たった時
/// </summary>
void Player::OnHitRoof()
{
	// Ｙ軸方向の速度は反転
	currentJumpPower = -currentJumpPower;
}

/// <summary>
/// 床に当たった時
/// </summary>
void Player::OnHitFloor()
{
	// Ｙ軸方向の移動速度は０に
	currentJumpPower = 0.0f;

	// もしジャンプ中だった場合は着地状態にする
	if (currentState == State::Jump)
	{
		// 移動していたかどうかで着地後の状態と再生するアニメーションを分岐する
		if (isMove)
		{
			// 移動している場合は走り状態に
			PlayAnim(AnimKind::Run);
			currentState = State::Run;
		}
		else
		{
			// 移動していない場合は立ち止り状態に
			PlayAnim(AnimKind::Stop);
			currentState = State::Stand;
		}

		// 着地時はアニメーションのブレンドは行わない
		animBlendRate = 1.0f;
	}
}

/// <summary>
/// 落下が確定したとき
/// </summary>
void Player::OnFall()
{
	if (currentState != State::Jump)
	{
		// ジャンプ中(落下中）にする
		currentState = State::Jump;

		// ちょっとだけジャンプする
		currentJumpPower = FallUpPower;

		// アニメーションは落下中のものにする
		PlayAnim(AnimKind::Jump);
	}
}