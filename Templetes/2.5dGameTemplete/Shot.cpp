#include "DxLib.h"
#include "GlobalConstants.h"
#include "Shot.h"
#include "Enemy.h"

namespace
{
	const static float	Speed		= 30.0f;
	const static float	AliveLimitX	= Graphics::ScreenW + 40.0f;
	const static float	HitSize		= 10;		// ショットの当たり判定サイズ

	const static VECTOR	ModelScale	= VGet(0.002f, 0.002f, 0.002f);
}

/// <summary>
///  コンストラクタ
/// </summary>
Shot::Shot()
	: pos			(VGet(0,0,0))
	 ,dir			(VGet(0, 0, 0))
	 ,isAlive		(false)
	 ,modelHandle	(-1)
{
	// 処理なし
}

/// <summary>
/// ショット初期化
/// </summary>
void Shot::Initialize()
{
	// --- 全部の弾で共通のデータ
	modelHandle = MV1LoadModel("data/model/Shot/SpikyBall.mv1");
	// 3Dモデルが大きすぎるので、スケールで調整
	MV1SetScale(modelHandle, ModelScale);

	// --- 弾の数だけ存在するデータ
	// 弾の位置、ディレクションを作成
	pos = VGet(0, 0, 0);
	dir = VGet(1, 0, 0);	// 弾は常に右にしか移動しない

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
		VECTOR shotVelocity = VScale(dir, Speed);
		pos = VAdd(pos, shotVelocity);

		// 画面外に出てしまった場合は存在状態を保持している変数にfalse(存在しない)を代入する
		if (pos.x > AliveLimitX)
		{
			isAlive = false;
		}

		// 円（または球）同士の当たり判定
		// ショットから敵への距離がお互いの当たり判定サイズより小さい＝当たっている
		// 弾が敵にぶつかっていたら、敵の状態をダメージ状態に
		VECTOR	shotToEnemy = VSub(enemy.GetPos(), pos);	// ショットから敵へのベクトル
		float	shotToEnemyLength = VSize(shotToEnemy);			// ショットから敵への距離
		if (shotToEnemyLength < Enemy::HitSize + HitSize)
		{
			enemy.OnDamage();

			// 弾も消す
			isAlive = false;
		}
	}
	// モデルのポジションを設定
	MV1SetPosition(modelHandle, Graphics::Get3DPosition(pos));
}

/// <summary>
///  ショット描画
/// </summary>
void Shot::Draw() const
{
	if (isAlive == true)
	{
		MV1DrawModel(modelHandle);

#if _DEBUG
		// デバッグ表示:弾の当たり判定の描画
		// 3D
		auto pos3D = Graphics::Get3DPosition(pos);
		SetUseLighting(FALSE);
		DrawSphere3D(pos3D, HitSize * Graphics::ScaleFactor3D, 4,
			Debug::ShotHitSizeColor, Debug::ShotHitSizeColor, FALSE);
		SetUseLighting(TRUE);
#endif
	}
}

/// <summary>
/// ショットが撃たれたとき
/// </summary>
void Shot::OnShoot(const VECTOR& shootPos)
{
	// 弾の発射位置をセット、撃たれた位置に
	pos = shootPos;

	// 弾が撃たれたので、存在状態を保持する変数にtrueを代入する
	isAlive = true;
}