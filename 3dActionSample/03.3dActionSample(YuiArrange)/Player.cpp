#include <math.h>
#include "DxLib.h"
#include "Camera.h"
#include "Input.h"
#include "Stage.h"
#include "Player.h"


/// <summary>
/// 初期化
/// </summary>
void Player::Initialize()
{
	// 初期座標は原点
	Position = VGet(0.0f, 0.0f, 0.0f);

	// 回転値は０
	Angle = 0.0f;

	// ジャンプ力は初期状態では０
	CurrentJumpPower = 0.0f;

	// モデルの読み込み
	ModelHandle = MV1LoadModel("DxChara.x");

	// 影描画用の画像の読み込み
	ShadowHandle = LoadGraph("Shadow.tga");

	// 初期状態では「立ち止り」状態
	State = State::STAND;

	// 初期状態でプレイヤーが向くべき方向はＸ軸方向
	TargetMoveDirection = VGet(1.0f, 0.0f, 0.0f);

	// アニメーションのブレンド率を初期化
	AnimBlendRate = 1.0f;

	// 初期状態ではアニメーションはアタッチされていないにする
	CurrentPlayAnim = -1;
	PrevPlayAnim = -1;

	// ただ立っているアニメーションを再生
	// TODO: マジックナンバーの4が何なのか解析して定数化
	PlayAnim(AnimKind::STOP);			// HACK: 中ではアタッチとカウンタの初期化をしているだけ
}

/// <summary>
/// 後始末
/// </summary>
void Player::Finalize()
{
	// モデルの削除
	MV1DeleteModel(ModelHandle);

	// 影用画像の削除
	DeleteGraph(ShadowHandle);
}

/// <summary>
/// 更新
/// </summary>
void Player::Update(const Input& input, const Camera& camera, Stage& stage)
{
	// ルートフレームのＺ軸方向の移動パラメータを無効にする
	DisableRootFrameZMove();

	// パッド入力によって移動パラメータを設定する
	VECTOR	UpMoveVec;		// 方向ボタン「↑」を入力をしたときのプレイヤーの移動方向ベクトル
	VECTOR	LeftMoveVec;	// 方向ボタン「←」を入力をしたときのプレイヤーの移動方向ベクトル
	VECTOR	MoveVec;		// このフレームの移動ベクトル
	auto IsPressMoveButton = UpdateMoveParameterWithPad(input, camera, UpMoveVec, LeftMoveVec, MoveVec);

	// 移動ボタンが押されたかどうかで処理を分岐
	if (IsPressMoveButton)
	{
		// 移動ベクトルを正規化したものをプレイヤーが向くべき方向として保存
		TargetMoveDirection = VNorm(MoveVec);

		// プレイヤーが向くべき方向ベクトルをプレイヤーのスピード倍したものを移動ベクトルとする
		MoveVec = VScale(TargetMoveDirection, MoveSpeed);

		// もし今まで「立ち止まり」状態だったら
		if (State == State::STAND)
		{
			// 走りアニメーションを再生する
			PlayAnim(AnimKind::RUN);

			// 状態を「走り」にする
			State = State::RUN;
		}
	}
	else
	{
		// このフレームで移動していなくて、且つ状態が「走り」だったら
		if (State == State::RUN)
		{
			// 立ち止りアニメーションを再生する
			PlayAnim(AnimKind::STOP);

			// 状態を「立ち止り」にする
			State = State::STAND;
		}
	}

	// 状態が「ジャンプ」の場合は
	if (State == State::JUMP)
	{
		// Ｙ軸方向の速度を重力分減算する
		CurrentJumpPower -= Gravity;

		// もし落下していて且つ再生されているアニメーションが上昇中用のものだった場合は
		if (CurrentJumpPower < 0.0f && MV1GetAttachAnim(ModelHandle, CurrentPlayAnim) == 2)
		{
			// 落下中ようのアニメーションを再生する
			PlayAnim(AnimKind::JUMP);
		}

		// 移動ベクトルのＹ成分をＹ軸方向の速度にする
		MoveVec.y = CurrentJumpPower;
	}

	// プレイヤーの移動方向にモデルの方向を近づける
	UpdateAngle();

	// 移動ベクトルを元にコリジョンを考慮しつつプレイヤーを移動
	Move(MoveVec, stage);

	// アニメーション処理
	UpdateAnimation();
}

/// <summary>
/// 描画
/// </summary>
void Player::Draw(const Stage& stage)
{
	MV1DrawModel(ModelHandle);
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

	MATRIX LocalMatrix;

	// ユーザー行列を解除する
	MV1ResetFrameUserLocalMatrix(ModelHandle, 2);

	// 現在のルートフレームの行列を取得する
	LocalMatrix = MV1GetFrameLocalMatrix(ModelHandle, 2);

	// Ｚ軸方向の平行移動成分を無効にする
	LocalMatrix.m[3][2] = 0.0f;

	// ユーザー行列として平行移動成分を無効にした行列をルートフレームにセットする
	MV1SetFrameUserLocalMatrix(ModelHandle, 2, LocalMatrix);
}

/// <summary>
/// パッド入力によって移動パラメータを設定する
/// </summary>
bool Player::UpdateMoveParameterWithPad(const Input& input, const Camera& camera, VECTOR& UpMoveVec, VECTOR& LeftMoveVec, VECTOR& MoveVec)
{
	// プレイヤーの移動方向のベクトルを算出
	// 方向ボタン「↑」を押したときのプレイヤーの移動ベクトルはカメラの視線方向からＹ成分を抜いたもの
	UpMoveVec = VSub(camera.GetTarget(), camera.GetEye());
	UpMoveVec.y = 0.0f;

	// 方向ボタン「←」を押したときのプレイヤーの移動ベクトルは上を押したときの方向ベクトルとＹ軸のプラス方向のベクトルに垂直な方向
	LeftMoveVec = VCross(UpMoveVec, VGet(0.0f, 1.0f, 0.0f));

	// 二つのベクトルを正規化( ベクトルの長さを１．０にすること )
	UpMoveVec = VNorm(UpMoveVec);
	LeftMoveVec = VNorm(LeftMoveVec);

	// このフレームでの移動ベクトルを初期化
	MoveVec = VGet(0.0f, 0.0f, 0.0f);

	// 移動したかどうかのフラグを初期状態では「移動していない」を表すFALSEにする
	bool IsPressMoveButton = false;

	// パッドの３ボタンと左シフトがどちらも押されていなかったらプレイヤーの移動処理
	if (CheckHitKey(KEY_INPUT_LSHIFT) == 0 && (input.GetNowFrameInput() & PAD_INPUT_C) == 0)
	{
		// 方向ボタン「←」が入力されたらカメラの見ている方向から見て左方向に移動する
		if (input.GetNowFrameInput() & PAD_INPUT_LEFT)
		{
			// 移動ベクトルに「←」が入力された時の移動ベクトルを加算する
			MoveVec = VAdd(MoveVec, LeftMoveVec);

			// 移動したかどうかのフラグを「移動した」にする
			IsPressMoveButton = true;
		}
		else
			// 方向ボタン「→」が入力されたらカメラの見ている方向から見て右方向に移動する
			if (input.GetNowFrameInput() & PAD_INPUT_RIGHT)
			{
				// 移動ベクトルに「←」が入力された時の移動ベクトルを反転したものを加算する
				MoveVec = VAdd(MoveVec, VScale(LeftMoveVec, -1.0f));

				// 移動したかどうかのフラグを「移動した」にする
				IsPressMoveButton = true;
			}

		// 方向ボタン「↑」が入力されたらカメラの見ている方向に移動する
		if (input.GetNowFrameInput() & PAD_INPUT_UP)
		{
			// 移動ベクトルに「↑」が入力された時の移動ベクトルを加算する
			MoveVec = VAdd(MoveVec, UpMoveVec);

			// 移動したかどうかのフラグを「移動した」にする
			IsPressMoveButton = true;
		}
		else
			// 方向ボタン「↓」が入力されたらカメラの方向に移動する
			if (input.GetNowFrameInput() & PAD_INPUT_DOWN)
			{
				// 移動ベクトルに「↑」が入力された時の移動ベクトルを反転したものを加算する
				MoveVec = VAdd(MoveVec, VScale(UpMoveVec, -1.0f));

				// 移動したかどうかのフラグを「移動した」にする
				IsPressMoveButton = true;
			}

		// プレイヤーの状態が「ジャンプ」ではなく、且つボタン１が押されていたらジャンプする
		if (State != State::JUMP && (input.GetNowFrameNewInput() & PAD_INPUT_A))
		{
			// 状態を「ジャンプ」にする
			State = State::JUMP;

			// Ｙ軸方向の速度をセット
			CurrentJumpPower = JumpPower;

			// ジャンプアニメーションの再生
			PlayAnim(AnimKind::JUMP);
		}
	}
	return IsPressMoveButton;
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
		IsMove = true;
	}
	else
	{
		IsMove = false;
	}

	// 当たり判定をして、新しい座標を保存する
	VECTOR OldPos = Position;							// 移動前の座標	
	VECTOR NextPos = VAdd(Position, MoveVector);		// 移動後の座標

	Position = stage.CheckCollision(*this, NextPos, MoveVector);

	// プレイヤーのモデルの座標を更新する
	MV1SetPosition(ModelHandle, Position);
}

// プレイヤーの回転制御
void Player::UpdateAngle()
{
	// プレイヤーの移動方向にモデルの方向を近づける
	float TargetAngle;			// 目標角度
	float difference;			// 目標角度と現在の角度との差

	// 目標の方向ベクトルから角度値を算出する
	TargetAngle = static_cast<float>(atan2(TargetMoveDirection.x, TargetMoveDirection.z));

	// 目標の角度と現在の角度との差を割り出す
	// 最初は単純に引き算
	difference = TargetAngle - Angle;

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
	Angle = TargetAngle - difference;
	MV1SetRotationXYZ(ModelHandle, VGet(0.0f, Angle + DX_PI_F, 0.0f));
}

// プレイヤーのアニメーションを再生する
void Player::PlayAnim(AnimKind PlayAnim)
{
	// HACK: 指定した番号のアニメーションをアタッチし、直前に再生していたアニメーションの情報をprevに移行している
	// 入れ替えを行うので、１つ前のモーションがが有効だったらデタッチする
	if (PrevPlayAnim != -1)
	{
		MV1DetachAnim(ModelHandle, PrevPlayAnim);
		PrevPlayAnim = -1;
	}

	// 今まで再生中のモーションだったものの情報をPrevに移動する
	PrevPlayAnim = CurrentPlayAnim;
	PrevAnimCount = CurrentAnimCount;

	// 新たに指定のモーションをモデルにアタッチして、アタッチ番号を保存する
	CurrentPlayAnim = MV1AttachAnim(ModelHandle, static_cast<int>(PlayAnim));
	CurrentAnimCount = 0.0f;

	// ブレンド率はPrevが有効ではない場合は１．０ｆ( 現在モーションが１００％の状態 )にする
	AnimBlendRate = PrevPlayAnim == -1 ? 1.0f : 0.0f;
}

// プレイヤーのアニメーション処理
void Player::UpdateAnimation()
{
	float AnimTotalTime;		// 再生しているアニメーションの総時間

	// ブレンド率が１以下の場合は１に近づける
	if (AnimBlendRate < 1.0f)
	{
		AnimBlendRate += AnimBlendSpeed;
		if (AnimBlendRate > 1.0f)
		{
			AnimBlendRate = 1.0f;
		}
	}

	// 再生しているアニメーション１の処理
	if (CurrentPlayAnim != -1)
	{
		// アニメーションの総時間を取得
		AnimTotalTime = MV1GetAttachAnimTotalTime(ModelHandle, CurrentPlayAnim);

		// 再生時間を進める
		CurrentAnimCount += PlayAnimSpeed;

		// 再生時間が総時間に到達していたら再生時間をループさせる
		if (CurrentAnimCount >= AnimTotalTime)
		{
			CurrentAnimCount = static_cast<float>(fmod(CurrentAnimCount, AnimTotalTime));
		}

		// 変更した再生時間をモデルに反映させる
		MV1SetAttachAnimTime(ModelHandle, CurrentPlayAnim, CurrentAnimCount);

		// アニメーション１のモデルに対する反映率をセット
		MV1SetAttachAnimBlendRate(ModelHandle, CurrentPlayAnim, AnimBlendRate);
	}

	// 再生しているアニメーション２の処理
	if (PrevPlayAnim != -1)
	{
		// アニメーションの総時間を取得
		AnimTotalTime = MV1GetAttachAnimTotalTime(ModelHandle, PrevPlayAnim);

		// 再生時間を進める
		PrevAnimCount += PlayAnimSpeed;

		// 再生時間が総時間に到達していたら再生時間をループさせる
		if (PrevAnimCount > AnimTotalTime)
		{
			PrevAnimCount = static_cast<float>(fmod(PrevAnimCount, AnimTotalTime));
		}

		// 変更した再生時間をモデルに反映させる
		MV1SetAttachAnimTime(ModelHandle, PrevPlayAnim, PrevAnimCount);

		// アニメーション２のモデルに対する反映率をセット
		MV1SetAttachAnimBlendRate(ModelHandle, PrevPlayAnim, 1.0f - AnimBlendRate);
	}
}

// プレイヤーの影を描画
void Player::DrawShadow(const Stage& stage)
{
	MV1_COLL_RESULT_POLY_DIM HitResultDim;
	MV1_COLL_RESULT_POLY* HitResult;
	VERTEX3D Vertex[3];
	VECTOR SlideVec;

	// ライティングを無効にする
	SetUseLighting(FALSE);

	// Ｚバッファを有効にする
	SetUseZBuffer3D(TRUE);

	// テクスチャアドレスモードを CLAMP にする( テクスチャの端より先は端のドットが延々続く )
	SetTextureAddressMode(DX_TEXADDRESS_CLAMP);

	// プレイヤーの直下に存在する地面のポリゴンを取得
	HitResultDim = MV1CollCheck_Capsule(stage.GetModelHandle(), -1, Position, VAdd(Position, VGet(0.0f, -ShadowHeight, 0.0f)), ShadowSize);

	// 頂点データで変化が無い部分をセット
	Vertex[0].dif = GetColorU8(255, 255, 255, 255);
	Vertex[0].spc = GetColorU8(0, 0, 0, 0);
	Vertex[0].su = 0.0f;
	Vertex[0].sv = 0.0f;
	Vertex[1] = Vertex[0];
	Vertex[2] = Vertex[0];

	// 球の直下に存在するポリゴンの数だけ繰り返し
	HitResult = HitResultDim.Dim;
	for (int i = 0; i < HitResultDim.HitNum; i++, HitResult++)
	{
		// ポリゴンの座標は地面ポリゴンの座標
		Vertex[0].pos = HitResult->Position[0];
		Vertex[1].pos = HitResult->Position[1];
		Vertex[2].pos = HitResult->Position[2];

		// ちょっと持ち上げて重ならないようにする
		SlideVec = VScale(HitResult->Normal, 0.5f);
		Vertex[0].pos = VAdd(Vertex[0].pos, SlideVec);
		Vertex[1].pos = VAdd(Vertex[1].pos, SlideVec);
		Vertex[2].pos = VAdd(Vertex[2].pos, SlideVec);

		// ポリゴンの不透明度を設定する
		Vertex[0].dif.a = 0;
		Vertex[1].dif.a = 0;
		Vertex[2].dif.a = 0;
		if (HitResult->Position[0].y > Position.y - ShadowHeight)
			Vertex[0].dif.a = static_cast<BYTE>(128 * (1.0f - static_cast<float>(fabs(HitResult->Position[0].y - Position.y) / ShadowHeight)));

		if (HitResult->Position[1].y > Position.y - ShadowHeight)
			Vertex[1].dif.a = static_cast<BYTE>(128 * (1.0f - static_cast<float>(fabs(HitResult->Position[1].y - Position.y) / ShadowHeight)));

		if (HitResult->Position[2].y > Position.y - ShadowHeight)
			Vertex[2].dif.a = static_cast<BYTE>(128 * (1.0f - static_cast<float>(fabs(HitResult->Position[2].y - Position.y) / ShadowHeight)));

		// ＵＶ値は地面ポリゴンとプレイヤーの相対座標から割り出す
		Vertex[0].u = (HitResult->Position[0].x - Position.x) / (ShadowSize * 2.0f) + 0.5f;
		Vertex[0].v = (HitResult->Position[0].z - Position.z) / (ShadowSize * 2.0f) + 0.5f;
		Vertex[1].u = (HitResult->Position[1].x - Position.x) / (ShadowSize * 2.0f) + 0.5f;
		Vertex[1].v = (HitResult->Position[1].z - Position.z) / (ShadowSize * 2.0f) + 0.5f;
		Vertex[2].u = (HitResult->Position[2].x - Position.x) / (ShadowSize * 2.0f) + 0.5f;
		Vertex[2].v = (HitResult->Position[2].z - Position.z) / (ShadowSize * 2.0f) + 0.5f;

		// 影ポリゴンを描画
		DrawPolygon3D(Vertex, 1, ShadowHandle, TRUE);
	}

	// 検出した地面ポリゴン情報の後始末
	MV1CollResultPolyDimTerminate(HitResultDim);

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
	CurrentJumpPower = -CurrentJumpPower;
}

/// <summary>
/// 床に当たった時
/// </summary>
void Player::OnHitFloor()
{
	// Ｙ軸方向の移動速度は０に
	CurrentJumpPower = 0.0f;

	// もしジャンプ中だった場合は着地状態にする
	if (State == State::JUMP)
	{
		// 移動していたかどうかで着地後の状態と再生するアニメーションを分岐する
		if (IsMove)
		{
			// 移動している場合は走り状態に
			PlayAnim(AnimKind::RUN);
			State = State::RUN;
		}
		else
		{
			// 移動していない場合は立ち止り状態に
			PlayAnim(AnimKind::STOP);
			State = State::STAND;
		}

		// 着地時はアニメーションのブレンドは行わない
		AnimBlendRate = 1.0f;
	}
}

/// <summary>
/// 落下が確定したとき
/// </summary>
void Player::OnFall()
{
	if (State != State::JUMP)
	{
		// ジャンプ中(落下中）にする
		State = State::JUMP;

		// ちょっとだけジャンプする
		CurrentJumpPower = FallUpPower;

		// アニメーションは落下中のものにする
		PlayAnim(AnimKind::JUMP);
	}
}