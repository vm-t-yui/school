//-----------------------------------------------------------------------------
// @brief  エネミー処理.
//-----------------------------------------------------------------------------
#include "EnemyParam.h"
#include "EnemyParamModel.h"
#include "EnemyUiHp.h"
#include "EnemyParamUI.h"
#include "StateMachine.h"
#include "Enemy.h"

const int EnemyFirstX = SCREEN_W / 2;
const int EnemyFirstY = 50;
const int EnemyMoveSpeed = 2;
const int EnemyDamageTime = 30;
const float EnemyDamageViveSpeed = 0.5f;
const float EnemyDamageViveHeight = 6.0f;

//----------------------------//
// 初期化.
//----------------------------//
void Enemy::Init(OnDamageDelegate onDamageDelegate)
{
	// 表示座標を初期化
	X = EnemyFirstX;
	Y = EnemyFirstY;

	// 右移動フラグ
	RightMove = true;

	// ダメージイベントの関数ポインタを保存
	onDamage = onDamageDelegate;

	// ステートマシンの初期化、Entry
	auto enter = [this]() { EnterNormal(); };
	auto update = [this]() { UpdateNormal(); };
	auto exit = [this]() { ExitNormal(); };
	stateMachine.AddState(Normal, enter, update, exit);
	stateMachine.AddState(Damage, [this]() {EnterDamage(); }, [this]() {UpdateDamage(); }, [this]() {ExitDamage(); });
	stateMachine.SetState(Normal);
}

// アップデート.
void Enemy::Update()
{
	stateMachine.Update();	// ステートマシンの更新
}

////// ステート:通常 ////
// 通常Enter
void Enemy::EnterNormal()
{
	// 通常状態の画像ロード
	Graph = LoadGraph("data/texture/enemy.png");
	GetGraphSize(Graph, &W, &H);	// エネミーのグラフィックのサイズを得る

}
// 通常Update
void Enemy::UpdateNormal()
{
	// 左右移動
	if (RightMove)
	{
		X += EnemyMoveSpeed;
	}
	else
	{
		X -= EnemyMoveSpeed;
	}
	if (X > SCREEN_W - W)
	{
		RightMove = false;
		X = SCREEN_W - W;
	}
	else if (X < 0)
	{
		X = 0;
		RightMove = true;
	}
}
// 通常Exit
void Enemy::ExitNormal()
{
	// 通常状態の画像アンロード
	DeleteGraph(Graph);
}

////// ステート:ダメージ ////
// ダメージEnter
void Enemy::EnterDamage()
{
	// ダメージ画像のロード
	Graph = LoadGraph("data/texture/enemyDamage.png");
	GetGraphSize(Graph, &W, &H);	// エネミーのグラフィックのサイズを得る

}
// ダメージUpdate
void Enemy::UpdateDamage()
{
	++DamageCounter;
	if (DamageCounter > EnemyDamageTime)
	{
		stateMachine.SetState(Normal);
	}
	else
	{
		// 移動せず上下に震える
		Y = EnemyFirstY + static_cast<int>(sinf(DamageCounter * EnemyDamageViveSpeed) * EnemyDamageViveHeight);
	}
}
// ダメージExit
void Enemy::ExitDamage()
{
	// ダメージ画像のアンロード
	DeleteGraph(Graph);
	Y = EnemyFirstY;
}

//----------------------------//
// 描画.
//----------------------------//
void Enemy::Draw(const EnemyParam& param)
{
	// ★データベースであるEnemyParamがどこに保持されているかを意識する必要がない
	if (param.Life > 0)
	{
		DrawGraph(X, Y, Graph, TRUE);
	}
}

/// <summary>
/// ダメージを受けたとき
/// </summary>
void Enemy::OnDamage(int damage)
{
	// エネミーの顔を歪めている時間を測るカウンタ変数に０を代入
	DamageCounter = 0;

	// ControllerであるparamUIにダメージ処理を飛ばしている
	// パラメータがあるかどうか、どこが持っているか等を知る必要がなく、ただControllerが必要とする関数を呼ぶだけ
	onDamage(damage);

	// ダメージステートに設定
	stateMachine.SetState(Damage);
}