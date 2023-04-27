// 2023 Takeru Yui All Rights Reserved.
#include "DxLib.h"
#include "EnemyBase.h"

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