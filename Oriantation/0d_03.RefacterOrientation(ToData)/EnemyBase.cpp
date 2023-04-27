// 2023 Takeru Yui All Rights Reserved.
#include "DxLib.h"
#include "EnemyBase.h"
#include "Common.h"

/// <summary>
/// 初期化
/// </summary>
void EnemyBase::Init(const Data& srcData)
{
	data = &srcData;

	x = data->firstX;
	y = data->firstY;
	life = data->life;
	speed = data->speed;
	damageFlag	= false;			// エネミーが顔を歪めているかどうかの変数に『歪めていない』を表すFALSEを代入
	rightMove	= true;

	// ロード
	graph		= LoadGraph(data->graphPath);
	damageGraph = LoadGraph(data->graphPath);
	
	GetGraphSize(graph, &w, &h);	// エネミーのグラフィックのサイズを得る

	// ダメージリアクションで色変更を行うので、初期化しておく
	GraphFilter(damageGraph, DX_GRAPH_FILTER_HSB, 0, 0, 0, 256);
}

/// <summary>
/// 描画
/// </summary>
void EnemyBase::Draw() const
{
	if (life > 0)
	{
		// ダメージを受けている場合はダメージ時のグラフィックを描画する
		if (damageFlag == true)
		{
			DrawGraph(x, y, damageGraph, TRUE);
		}
		else
		{
			DrawGraph(x, y, graph, TRUE);
		}
	}
}

/// <summary>
/// 移動
/// </summary>
void EnemyBase::Move()
{
	// エネミーの座標を移動している方向に移動する
	if (rightMove == true)
	{
		if (life < data->speedUpLifeLimit)
		{
			x += speed * 2;
		}
		else
		{
			x += speed;
		}
	}
	else
	{
		if (life < data->speedUpLifeLimit)
		{
			x -= speed * 2;
		}
		else
		{
			x -= speed;
		}
	}
}

/// <summary>
/// 更新
/// </summary>
void EnemyBase::Update()
{
	// 拡張可能なMoveクラスを基底で呼び出すことで、移動に幅を持たせる
	Move();

	// エネミーが画面端からでそうになっていたら画面内の座標に戻してあげ、移動する方向も反転する
	if (x > SCREEN_W - w)
	{
		x = SCREEN_W - w;
		rightMove = false;
	}
	else if (x < 0)
	{
		x = 0;
		rightMove = true;
	}

	// ダメージを受けているかどうかで処理を分岐
	if (damageFlag == true)
	{
		damageCounter++;

		if (damageCounter == 5)
		{
			// 『ダメージをうけていない』を表すFALSEを代入
			damageFlag = false;
		}
	}
}

/// <summary>
/// ダメージを受けた
/// </summary>
void EnemyBase::OnDamage()
{
	// エネミーの顔を歪めているかどうかを保持する変数に『歪めている』を表すTRUEを代入
	damageFlag = true;

	// エネミーの顔を歪めている時間を測るカウンタ変数に０を代入
	damageCounter = 0;

	life -= 1;
}