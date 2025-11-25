#include "DxLib.h"
#include <vector>
#include "GlobalConstants.h"
#include "Player.h"
#include "Input.h"
#include "Shot.h"

/// <summary>
/// プレイヤーの初期化
/// </summary>
void Player::Initialize()
{
	// プレイヤーのグラフィックをメモリにロード＆表示座標を初期化
	pos			= Player::FirstPos;
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
}

/// <summary>
/// プレイヤーの更新
/// </summary>
void Player::Update(const Input& input, std::vector<Shot>& shotArray)
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
		dir = VAdd(dir, VGet(0, -1, 0));
	}
	if (CheckHitKey(KEY_INPUT_DOWN) == 1)
	{
		dir = VAdd(dir, VGet(0, 1, 0));
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
	VECTOR playerVelocity = VScale(dir, Player::Speed);	// 長さ1の向きに、大きさ（速度）をかける
	pos = VAdd(pos, playerVelocity);				// 座標ベクトルに、velocityを足すことで移動

	// 弾の発射処理
	// ボタンが押された瞬間だけ、発射処理を行う
	if (input.isPressedShotButton)
	{
		// 弾i個分繰り返す
		for (int i = 0; i < Shot::Num; i++)
		{
			// 弾が画面上にでていない場合はその弾を画面に出す
			if (shotArray[i].isAlive == false)
			{
				// 弾の発射位置をセット、プレイヤーの中心にする
				shotArray[i].pos = pos;

				// 弾が撃たれたので、存在状態を保持する変数にtrueを代入する
				shotArray[i].isAlive = true;

				// 弾の発射に合わせて攻撃アニメーションを流す
				ChangeAnimation(AttackAnimationIndex, attackAnimationModelHandle, AttackAnimationSpeed, false);

				break;	// 一発撃ったら抜ける
			}
		}
	}

	// プレイヤーが画面左端からはみ出そうになっていたら画面内の座標に戻してあげる
	if (pos.x < HitSize)
	{
		pos.x = HitSize;
	}
	if (pos.x > Graphics::ScreenW - HitSize)
	{
		pos.x = Graphics::ScreenW - HitSize;
	}
	if (pos.y < HitSize)
	{
		pos.y = HitSize;
	}
	if (pos.y > Graphics::ScreenH - HitSize)
	{
		pos.y = Graphics::ScreenH - HitSize;
	}

	// モデルのポジションを設定
	auto drawPos3D = Graphics::Get3DPosition(pos);
	drawPos3D = VAdd(drawPos3D, ModelFixPos);
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
	auto pos3D = Graphics::Get3DPosition(pos);
	SetUseLighting(FALSE);
	DrawSphere3D(pos3D, Player::HitSize * Graphics::ScaleFactor3D, 4,
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