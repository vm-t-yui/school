//-----------------------------------------------------------------------------
// @brief  エネミー処理.
//-----------------------------------------------------------------------------
#include"EnemyUiHp.h"
#include"EnemyParam.h"
#include"EnemyParamUI.h"
#include"enemy.h"

//----------------------------//
// 初期化.
//----------------------------//
void Enemy::Init()
{
	// エネミーのグラフィックをメモリにロード＆表示座標を初期化
	char* enemyGlaphStr = "data/texture/EpicEnemy.png";
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
}

//----------------------------//
// アップデート.
//----------------------------//
void Enemy::Update(EnemyParamUI& enemyParamUI)
{
	// エネミーの座標を移動している方向に移動する
	if (RightMove == true)
	{
		X += 3;
	}
	else
	{
		X -= 3;
	}

	// エネミーが画面端からでそうになっていたら画面内の座標に戻してあげ、移動する方向も反転する
	if (X > SCREEN_W - W)
	{
		X = SCREEN_W - W;
		RightMove = false;
	}
	else if (X < 0)
	{
		X = 0;
		RightMove = true;
	}

	// エネミーを描画
	// ダメージを受けているかどうかで処理を分岐
	if (DamageFlag == true)
	{
		auto param = enemyParamUI.GetParam();
		param.Life = param.Life - 1;

		DamageCounter++;

		if (DamageCounter == 5)
		{
			// 『ダメージをうけていない』を表すFALSEを代入
			DamageFlag = false;
		}

		enemyParamUI.OnChangeParam(param);
	}
}

//----------------------------//
// 描画.
//----------------------------//
void Enemy::Draw(const EnemyParam& param)
{
	if (param.Life > 0)
	{
		// ダメージを受けている場合はダメージ時のグラフィックを描画する
		if (DamageFlag == true)
		{
			DrawGraph(X, Y, DamageGraph, TRUE);
		}
		else
		{
			DrawGraph(X, Y, Graph, TRUE);
		}
	}
}