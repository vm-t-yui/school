#include "DxLib.h"
#include <vector>
#include <memory>
#include <assert.h>
#include "GlobalConstants.h"
#include "VirtualCamera.h"
#include "HardLockCamera.h"
#include "VirtualCameraManager.h"
#include "Player.h"
#include "Input.h"

namespace
{
	const static float	Speed			= 0.1f;
	const static float	HitSize			= 1.7f;			// 当たり判定サイズ（直径）

	// 当たり判定の修正位置。基準位置は足元なので、ヒットサイズ分上げる
	const static VECTOR	HitFixPos		= VGet(0.0f, HitSize * 0.5f, 0.0f);

	const static int	IdleAnimationIndex	 = 1;		// 待機アニメーションインデックス
	const static float	IdleAnimationSpeed	 = 0.3f;	// 待機アニメーションスピード
	const static int	AttackAnimationIndex = 1;		// 攻撃アニメーションインデックス
	const static float	AttackAnimationSpeed = 4.5f;	// 攻撃アニメーションスピード

	const static VECTOR	FirstPos		= VGet(0.0f, 0.0f, 0.0f);

	// モデルの表示スケールと位置補正
	const static VECTOR	ModelScale		= VGet(0.017f, 0.017f, 0.017f);
	const static VECTOR	ModelFixPos		= VGet(0.0f, 0.0f, 0.0f);

	// カメラ位置
	const static VECTOR	CameraPos1		= VGet(10.0f, 10.0f, -20.0f);
	const static VECTOR	CameraPos2		= VGet(-10.0f, -10.0f, -20.0f);
}

/// <summary>
/// コンストラクタ
/// </summary>
Player::Player()
	: pos							(VGet(0, 0, 0))
	 ,hitCenterPos					(HitFixPos)
	 ,dir							(VGet(0, 0, 0))
	 ,state							(State::Normal)
	 ,modelHandle					(-1)
	 ,idleAnimationModelHandle		(-1)
	 ,attackAnimationModelHandle	(-1)
	 ,currentAnimAttachIndex		(-1)
	 ,currentAnimTotalTime			(0)
	 ,currentAnimTimeCount			(0)
	 ,currentAnimSpeed				(0)
	 ,currentAnimIsLoop				(true)
	 ,isAnimEnd						(true)
	 ,isFinalized					(false)
{
	// 処理なし
}

/// <summary>
/// デストラクタ
/// </summary>
Player::~Player()
{
	if (!isFinalized)
	{
		assert(!"Finalizeが呼ばれていない");
	}
}

/// <summary>
/// プレイヤーの初期化
/// </summary>
void Player::Initialize(VirtualCameraManager& virtualCameraManager)
{
	// プレイヤーのグラフィックをメモリにロード＆表示座標を初期化
	pos			= FirstPos;
	dir			= VGet(0, 0, 0);	// プレイヤーの向き

	// 3Dモデルとアニメーションのロード
	// 3Dモデルが入っているだけのモデルファイルと、モデルなしでアニメーションだけが入っているモデルファイルをロードする
	modelHandle					= MV1LoadModel("data/model/player/Player_Model.mv1");
	idleAnimationModelHandle	= MV1LoadModel("data/model/player/Player_Idle.mv1");
	attackAnimationModelHandle	= MV1LoadModel("data/model/player/Player_Attack.mv1");

	// 待機アニメーションをアタッチ
	// モデルだけのファイルに、アニメーションだけのファイルの内容をアタッチする
	ChangeAnimation(IdleAnimationIndex, idleAnimationModelHandle, IdleAnimationSpeed, true);

	// 3Dモデルが大きすぎるので、スケールで調整
	MV1SetScale(modelHandle, ModelScale);

	// 横を向かせる
	float rotateX = 0.0f * (DX_PI_F / 180.0f);
	float rotateY = -90.0f * (DX_PI_F / 180.0f);
	float rotateZ = 0.0f * (DX_PI_F / 180.0f);
	MV1SetRotationXYZ(modelHandle, VGet(rotateX, rotateY, rotateZ));

	// カメラ初期化
	camera1 = std::make_shared<HardLockCamera>(CameraPos1, VGet(0, 0, 0), 60.0f, 1, true);
	camera2 = std::make_shared<HardLockCamera>(CameraPos2, VGet(0, 0, 0), 60.0f, 2, true);

	// カメラ登録
	virtualCameraManager.Register(camera1);
	virtualCameraManager.Register(camera2);
}

/// <summary>
/// 終了処理
/// </summary>
void Player::Finalize(VirtualCameraManager& virtualCameraManager)
{
	virtualCameraManager.Remove(camera1);
	virtualCameraManager.Remove(camera2);
	camera1 = nullptr;
	camera2 = nullptr;
	isFinalized = true;
}


/// <summary>
/// プレイヤーの更新
/// </summary>
void Player::Update(const Input& input)
{
	// アニメーション時間の更新
	currentAnimTimeCount += currentAnimSpeed;
	if (currentAnimIsLoop)
	{
		// 総再生時間を越えたらゼロに戻すことでループ
		if (currentAnimTimeCount > currentAnimTotalTime)
		{
			currentAnimTimeCount = 0.0f;
		}
	}
	else
	{
		// 最後の時間で止める
		if (currentAnimTimeCount > currentAnimTotalTime)
		{
			currentAnimTimeCount = currentAnimTotalTime;
			isAnimEnd = true;

			// アニメーションが止まってたらとりあえずIdleのモーションに戻す
			ChangeAnimation(IdleAnimationIndex, idleAnimationModelHandle, IdleAnimationSpeed, true);
		}
	}
	MV1SetAttachAnimTime(modelHandle, currentAnimAttachIndex, currentAnimTimeCount);

	dir = VGet(0, 0, 0);	// 向きをリセット

	// 矢印キーを押していたらプレイヤーを移動させる
	if (CheckHitKey(KEY_INPUT_UP) == 1)
	{
		dir = VAdd(dir, VGet(0, 1, 0));
	}
	if (CheckHitKey(KEY_INPUT_DOWN) == 1)
	{
		dir = VAdd(dir, VGet(0, -1, 0));
	}
	if (CheckHitKey(KEY_INPUT_LEFT) == 1)
	{
		dir = VAdd(dir, VGet(-1, 0, 0));
	}
	if (CheckHitKey(KEY_INPUT_RIGHT) == 1)
	{
		dir = VAdd(dir, VGet(1, 0, 0));
	}

	// 長さがゼロじゃない場合、向きを正規化して、長さ1に
	if (VSize(dir) > 0)
	{
		dir = VNorm(dir);
	}

	// プレイヤーの移動
	VECTOR playerVelocity = VScale(dir, Speed);	// 長さ1の向きに、大きさ（速度）をかける
	pos = VAdd(pos, playerVelocity);				// 座標ベクトルに、velocityを足すことで移動

	// 弾の発射処理
	// ボタンが押された瞬間だけ、発射処理を行う
	if (input.IsPressedShotButton())
	{
		// 弾の発射に合わせて攻撃アニメーションを流してたとこの名残り。アニメーションだけ流す
		ChangeAnimation(AttackAnimationIndex, attackAnimationModelHandle, AttackAnimationSpeed, false);
	}

	// 1キーでカメラ１、2キーでカメラ２に切り替え
	if (CheckHitKey(KEY_INPUT_1) == 1)
	{
		camera1->SetActive(true);
		camera2->SetActive(false);
	}
	else if (CheckHitKey(KEY_INPUT_2) == 1)
	{
		camera1->SetActive(false);
		camera2->SetActive(true);
	}

	// カメラ更新
	camera1->Update();
	camera2->Update();

	// 位置合わせ後にヒットポジションを補正
	hitCenterPos = VAdd(pos, HitFixPos);

	// モデルのポジションを設定
	auto drawPos3D = VAdd(pos, ModelFixPos);
	MV1SetPosition(modelHandle, drawPos3D);
}

/// <summary>
/// プレイヤーを描画
/// </summary>
void Player::Draw() const
{
	MV1DrawModel(modelHandle);

#if _DEBUG
	// デバッグ表示:敵の当たり判定の描画
	// 3D
	SetUseLighting(FALSE);
	// 足元基準なので当たり判定位置は半径分ずらす
	DrawSphere3D(hitCenterPos, HitSize * 0.5f, 4,
		Debug::PlayerHitSizeColor, Debug::EnemyHitSizeColor, FALSE);
	SetUseLighting(TRUE);
#endif
}


/// <summary>
/// アニメーションの切り替え
/// </summary>
/// <param name="animationIndex">再生アニメーションインデックス</param>
/// <param name="animationIndex">アニメーションモデルのハンドル</param>
/// <param name="speed">スピード</param>
/// <param name="isLoop">ループするか</param>
void Player::ChangeAnimation(int animationIndex, int sourceAnimationModelHandle,float speed, bool isLoop)
{
	// 古いアニメーションがあればデタッチ
	if (currentAnimAttachIndex >= 0)
	{
		MV1DetachAnim(modelHandle, currentAnimAttachIndex);
	}
	// アタッチ
	// 	int MV1AttachAnim( int MHandle, int AnimIndex, int AnimSrcMHandle, int NameCheck ) ;
	// 今までAnimSrcMHandle=-1として、同じモデルファイルの中にアニメーションがある前提だったが
	// 別モデルファイルからアニメーションをモデルにアタッチしている
	// この場合、animationIndexはアニメーションファイル側のインデックス
	currentAnimAttachIndex	= MV1AttachAnim(modelHandle, animationIndex, sourceAnimationModelHandle, FALSE);
	currentAnimTotalTime	= MV1GetAttachAnimTotalTime(modelHandle, currentAnimAttachIndex);
	currentAnimTimeCount	= 0.0f;
	currentAnimSpeed		= speed;
	currentAnimIsLoop		= isLoop;
	isAnimEnd				= false;
}