//-----------------------------------------------------------------------------
// @brief  エネミー処理.
//-----------------------------------------------------------------------------
#include "EnemyParam.h"
#include "EnemyParamModel.h"
#include "EnemyUiHp.h"
#include "EnemyParamUI.h"
#include "StateMachine.h"
#include "Enemy.h"

//----------------------------//
// 初期化.
//----------------------------//
void Enemy::Init(OnDamageDelegate onDamageDelegate)
{
	// エネミーのグラフィックをメモリにロード＆表示座標を初期化
	char* enemyGlaphStr = (char*)"data/texture/EpicEnemy.png";
	Graph = LoadGraph(enemyGlaphStr);
	DamageGraph = LoadGraph(enemyGlaphStr);
	GraphFilter(DamageGraph, DX_GRAPH_FILTER_HSB, 0, 0, 0, 256);
	X = 0;
	Y = 50;

	// エネミーが顔を歪めているかどうかの変数に『歪めていない』を表すFALSEを代入
	DamageFlag = false;

	// エネミーのグラフィックのサイズを得る
	GetGraphSize(Graph, &W, &H);

	RightMove = true;

	// ダメージイベントの関数ポインタを保存
	onDamage = onDamageDelegate;

	// TODO: ステートマシンの初期化、Entry
	auto enter = [this]() { EnterNormal(); };
	auto update = [this]() { UpdateNormal(); };
	auto exit = [this](){ ExitNormal(); };
	stateMachine.AddState(Normal, enter, update, exit);
}

// アップデート.
void Enemy::Update()
{
	// TODO: ステートマシンの更新
	stateMachine.Update();
}

////// ステート:通常 ////
// 通常Enter
void Enemy::EnterNormal()
{
	// TODO:通常状態の画像ロード
}
// 通常Update
void Enemy::UpdateNormal()
{
	// TODO:左右移動
}
// 通常Exit
void Enemy::ExitNormal()
{
	// TODO:通常状態の画像アンロード
}

////// ステート:ダメージ ////
// ダメージEnter
void Enemy::EnterDamage()
{
	// TODO:ダメージ画像のロード
}
// ダメージUpdate
void Enemy::UpdateDamage()
{
	// TODO:移動しないとか上下に震えるとか
}
// ダメージExit
void Enemy::ExitDamage()
{
	// TODO:ダメージ画像のアンロード
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
	// エネミーの顔を歪めているかどうかを保持する変数に『歪めている』を表すTRUEを代入
	DamageFlag = true;

	// エネミーの顔を歪めている時間を測るカウンタ変数に０を代入
	DamageCounter = 0;

	// ControllerであるparamUIにダメージ処理を飛ばしている
	// パラメータがあるかどうか、どこが持っているか等を知る必要がなく、ただControllerが必要とする関数を呼ぶだけ
	onDamage(damage);
}