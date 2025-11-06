#include "DxLib.h"
#include "Enemy.h"
#include "GlobalConstants.h"

/// <summary>
/// 敵の初期化
/// </summary>
void Enemy::Initialize()
{
	// エネミーのグラフィックをメモリにロード＆表示座標を初期化
	pos = Enemy::FirstPos;
	dir = VGet(0, 0, 0);	// エネミーの向き
	graphNormal = LoadGraph("data/texture/enemy.png");
	graphDamage = LoadGraph("data/texture/enemyDamage.png");
	graph = graphNormal;

	//////////////////////////////////////////////////////
	modelHandle = MV1LoadModel("data/model/Enemy/Enemy.mv1");
	// 3Dモデルが大きすぎるので、スケールで調整
	MV1SetScale(modelHandle, ModelScale);
	//////////////////////////////////////////////////////

	GetGraphSize(graph, &w, &h);

	// エネミーが右移動しているかどうかのフラグをリセット
	isRightMove = true;
	state = Enemy::State::Normal;		// 通常状態で初期化
	damageCount = 0;						// ダメージカウントを初期化
}

/// <summary>
/// エネミーの更新
/// </summary>
void Enemy::Update()
{
	// エネミーの移動方向の確定。固定で必ず左右どちらかになる
	if (isRightMove == true)
	{
		dir = VGet(1, 0, 0);
	}
	else
	{
		dir = VGet(-1, 0, 0);
	}

	// エネミーの状態別処理
	switch (state)
	{
	case Enemy::State::Normal:	// 通常なら通常顔に
		graph = graphNormal;
		break;
	case Enemy::State::Damage:	// ダメージならダメージ顔に。ダメージカウントを小さくする
		graph = graphDamage;
		--damageCount;		// カウントを減らし、ゼロ以下になったら通常状態に戻す
		if (damageCount <= 0)
		{
			damageCount = 0;
			state = Enemy::State::Normal;
		}
		break;
	default:	// それ以外なら何もしない
		break;
	}

	const float enemyHalfW = w * 0.5f;
	const float enemyHalfH = h * 0.5f;

	// エネミーの移動。すでに長さ１なので正規化はいらない
	VECTOR enemyVelocity = VScale(dir, Enemy::Speed);	// 長さ1の向きに、大きさ（速度）をかける
	pos = VAdd(pos, enemyVelocity);				// 座標ベクトルに、velicityを足すことで移動

	// エネミーが画面端からでそうになっていたら画面内の座標に戻してあげ、移動する方向も反転する
	if (pos.x > Graphics::ScreenW - enemyHalfW)
	{
		pos.x = Graphics::ScreenW - enemyHalfW;
		isRightMove = false;
	}
	else if (pos.x < enemyHalfW)
	{
		pos.x = enemyHalfW;
		isRightMove = true;
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
	const float enemyHalfW = w * 0.5f;
	const float enemyHalfH = h * 0.5f;

#if _DEBUG
	// エネミーを描画
	DrawRotaGraph3(static_cast<int>(pos.x),
		static_cast<int>(pos.y),
		static_cast<int>(enemyHalfW), static_cast<int>(enemyHalfH),
		1.0f, 1.0f,
		0.0f, graph,
		FALSE, FALSE);
#endif

	// モデル描画
	MV1DrawModel(modelHandle);

#if _DEBUG
	// デバッグ表示:敵の当たり判定の描画
	// 2D
	DrawCircle(static_cast<int>(pos.x),
		static_cast<int>(pos.y),
		static_cast<int>(Enemy::HitSize),
		Debug::EnemyHitSizeColor, 0);

	////////////////////////////////////////////////
	// 3D
	auto pos3D = Graphics::Get3DPosition(pos);
	SetUseLighting(FALSE);
	DrawSphere3D(pos3D, Enemy::HitSize * Graphics::ScaleFactor3D, 4,
		Debug::EnemyHitSizeColor, Debug::EnemyHitSizeColor, FALSE);
	SetUseLighting(TRUE);
	////////////////////////////////////////////////
#endif
}