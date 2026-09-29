#include "DxLib.h"
#include "Enemy.h"
#include "GlobalConstants.h"

namespace
{
	const static VECTOR	FirstPos			 = VGet(1400, 300, 0);

	// モデルの表示スケールと位置補正
	const static VECTOR	ModelScale			 = VGet(0.003f, 0.003f, 0.003f);
	const static VECTOR	ModelFixPos			 = VGet(0.0f, -0.25f, 0.0f);

	const static int	DamageTime			 = 30;		// エネミーのダメージ顔になっている時間
	const static float	Speed				 = 3.0f;
	const static int	IdleAnimationIndex	 = 2;		// 待機アニメーションインデックス
	const static float	IdleAnimationSpeed	 = 0.3f;	// 待機アニメーションスピード
	const static int	DamageAnimationIndex = 4;		// ダメージアニメーションインデックス
	const static float	DamageAnimationSpeed = 0.8f;	// ダメージアニメーションスピード
}

/// <summary>
/// コンストラクタ
/// </summary>
Enemy::Enemy()
	: modelHandle			(-1)
	 ,currentAnimAttachIndex(-1)
	 ,currentAnimTotalTime	(0)
	 ,currentAnimTimeCount	(0)
	 ,currentAnimSpeed		(0)
	 ,currentAnimIsLoop		(true)
	 ,pos					(FirstPos)
	 ,dir					(VGet(0,0,0))
	 ,isUpMove				(true)
	 ,state					(State::Normal)
	 ,damageCount			(0)
{
	// 処理なし
}

/// <summary>
/// 敵の初期化
/// </summary>
void Enemy::Initialize()
{
	// エネミーのグラフィックをメモリにロード＆表示座標を初期化
	pos = FirstPos;
	dir = VGet(0, 0, 0);	// エネミーの向き

	modelHandle = MV1LoadModel("data/model/Enemy/Enemy.mv1");

	// 3Dモデルが大きすぎるので、スケールで調整
	MV1SetScale(modelHandle, ModelScale);

	// アニメーションを設定
	currentAnimAttachIndex = -1;	// まず空の状態を作る
	ChangeAnimation(IdleAnimationIndex, IdleAnimationSpeed, true);

	isUpMove	= true;							// エネミーが上移動しているかどうかのフラグをリセット
	state		= State::Normal;		// 通常状態で初期化
	damageCount	= 0;						// ダメージカウントを初期化
}

/// <summary>
/// エネミーの更新
/// </summary>
void Enemy::Update()
{
	//////////////////////////////////////////////////////
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
		}
	}
	MV1SetAttachAnimTime(modelHandle, currentAnimAttachIndex, currentAnimTimeCount);
	//////////////////////////////////////////////////////

	// エネミーの移動方向の確定。固定で必ず左右どちらかになる
	if (isUpMove == true)
	{
		dir = VGet(0, -1, 0);
	}
	else
	{
		dir = VGet(0, 1, 0);
	}

	// エネミーの状態別処理
	switch (state)
	{
	case Enemy::State::Normal:
		break;
	case Enemy::State::Damage:	// ダメージカウントを小さくする
		--damageCount;		// カウントを減らし、ゼロ以下になったら通常状態に戻す
		if (damageCount <= 0)
		{
			damageCount = 0;
			state = Enemy::State::Normal;
			ChangeAnimation(IdleAnimationIndex, IdleAnimationSpeed, true);
		}
		break;
	default:	// それ以外なら何もしない
		break;
	}

	// エネミーの移動。すでに長さ１なので正規化はいらない
	VECTOR enemyVelocity = VScale(dir, Speed);	// 長さ1の向きに、大きさ（速度）をかける
	pos = VAdd(pos, enemyVelocity);				// 座標ベクトルに、velocityを足すことで移動

	// エネミーが画面端からでそうになっていたら画面内の座標に戻してあげ、移動する方向も反転する
	if (pos.y > Graphics::ScreenH - HitSize)
	{
		pos.y = Graphics::ScreenH - HitSize;
		isUpMove = true;
	}
	else if (pos.y < HitSize)
	{
		pos.y = HitSize;
		isUpMove = false;
	}

	// モデルのポジションを設定
	auto drawPos3D = Graphics::Get3DPosition(pos);
	drawPos3D = VAdd(drawPos3D, ModelFixPos);
	MV1SetPosition(modelHandle, drawPos3D);
}

/// <summary>
/// エネミー描画
/// </summary>
void Enemy::Draw() const
{
	// モデル描画
	MV1DrawModel(modelHandle);

#if _DEBUG
	// デバッグ表示:敵の当たり判定の描画
	auto pos3D = Graphics::Get3DPosition(pos);
	SetUseLighting(FALSE);
	DrawSphere3D(pos3D, Enemy::HitSize * Graphics::ScaleFactor3D, 4,
		Debug::EnemyHitSizeColor, Debug::EnemyHitSizeColor, FALSE);
	SetUseLighting(TRUE);
#endif
}

/// <summary>
/// ダメージを受けたとき
/// </summary>
void Enemy::OnDamage()
{
	state = State::Damage;
	damageCount = DamageTime;

	// ダメージのアニメーションを設定
	ChangeAnimation(DamageAnimationIndex, DamageAnimationSpeed, false);
}

/// <summary>
/// アニメーションの切り替え
/// </summary>
/// <param name="animationIndex"></param>
/// <param name="speed">スピード</param>
/// <param name="isLoop">ループするか</param>
void Enemy::ChangeAnimation(int animationIndex, float speed, bool isLoop)
{
	// 古いアニメーションがあればデタッチ
	if (currentAnimAttachIndex >= 0)
	{
		MV1DetachAnim(modelHandle, currentAnimAttachIndex);
	}
	// アタッチ
	currentAnimAttachIndex	= MV1AttachAnim(modelHandle, animationIndex, -1, FALSE);
	currentAnimTotalTime	= MV1GetAttachAnimTotalTime(modelHandle, currentAnimAttachIndex);
	currentAnimTimeCount	= 0.0f;
	currentAnimSpeed		= speed;
	currentAnimIsLoop		= isLoop;
}