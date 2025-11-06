#include "DxLib.h"
#include "Shot.h"
#include "Enemy.h"
#include "GlobalConstants.h"

/// <summary>
/// ショット初期化
/// </summary>
void Shot::Initialize()
{
	// --- 全部の弾で共通のデータ
	// ショットのグラフィックをメモリにロード+サイズ取得
	graph = LoadGraph("data/texture/shot.png");

	GetGraphSize(graph, &w, &h);

	//////////////////////////////////////////////////////
	modelHandle = MV1LoadModel("data/model/Shot/SpikyBall.mv1");
	// 3Dモデルが大きすぎるので、スケールで調整
	MV1SetScale(modelHandle, ModelScale);
	//////////////////////////////////////////////////////

	// --- 弾の数だけ存在するデータ
	// 弾の位置、ディレクションを作成
	pos = VGet(0, 0, 0);
	dir = VGet(0, -1, 0);	// 弾は常に上にしか移動しない

	// 弾が画面上に存在しているか保持する変数に『存在していない』を意味するfalseを代入しておく
	isAlive = false;
}

/// <summary>
/// ショットの更新
/// </summary>
void Shot::Update(Enemy& enemy)
{
	// 存在状態を保持している変数の内容がtrue(存在する)の場合のみ行う
	if (isAlive == true)
	{
		// 弾を移動させる。dirは常に上方向で長さ１なので、正規化はいらない
		VECTOR shotVelocity = VScale(dir, Shot::Speed);
		pos = VAdd(pos, shotVelocity);

		// 画面外に出てしまった場合は存在状態を保持している変数にfalse(存在しない)を代入する
		if (pos.y < Shot::AliveLimitY)
		{
			isAlive = false;
		}

		// 弾が敵にぶつかっていたら、敵の状態をダメージ状態に
		VECTOR	shotToEnemy = VSub(enemy.pos, pos);	// ショットから敵へのベクトル
		float	shotToEnemyLength = VSize(shotToEnemy);			// ショットから敵への距離
		if (shotToEnemyLength < Enemy::HitSize + Shot::HitSize)
		{
			// 円（または球）同士の当たり判定
			// ショットから敵への距離がお互いの当たり判定サイズより小さい＝当たっている
			enemy.state = Enemy::State::Damage;
			enemy.damageCount = Enemy::DamageTime;

			// ダメージのアニメーションを設定
			enemy.ChangeAnimation(Enemy::DamageAnimationIndex, Enemy::DamageAnimationSpeed, false);

			// 弾も消す
			isAlive = false;
		}
	}

	//////////////////////////////////////////////////////
	// モデルのポジションを設定
	MV1SetPosition(modelHandle, Graphics::Get3DPosition(pos));
	//////////////////////////////////////////////////////
}

/// <summary>
///  ショット描画
/// </summary>
void Shot::Draw() const
{
	if (isAlive == true)
	{
#if _DEBUG
		// 弾を描画する
		const float shotHalfW = w * 0.5f;
		const float shotHalfH = h * 0.5f;
		DrawRotaGraph3(static_cast<int>(pos.x),
			static_cast<int>(pos.y),
			static_cast<int>(shotHalfW), static_cast<int>(shotHalfH),
			1.0f, 1.0f,
			0.0f, graph,
			FALSE, FALSE);
#endif

		//////////////////////////////////////////////////////
		MV1DrawModel(modelHandle);
		//////////////////////////////////////////////////////

#if _DEBUG
		// デバッグ表示:弾の当たり判定の描画
		DrawCircle(static_cast<int>(pos.x),
			static_cast<int>(pos.y),
			static_cast<int>(Shot::HitSize),
			Debug::ShotHitSizeColor, 0);

		//////////////////////////////////////////////////////
		// 3D
		auto pos3D = Graphics::Get3DPosition(pos);
		SetUseLighting(FALSE);
		DrawSphere3D(pos3D, Shot::HitSize * Graphics::ScaleFactor3D, 4,
			Debug::ShotHitSizeColor, Debug::ShotHitSizeColor, FALSE);
		SetUseLighting(TRUE);
		//////////////////////////////////////////////////////
#endif
	}
}