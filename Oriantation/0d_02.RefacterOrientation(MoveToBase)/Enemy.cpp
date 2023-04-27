// 2023 Takeru Yui All Rights Reserved.
#include "DxLib.h"
#include "EnemyBase.h"
#include "Enemy.h"

/// <summary>
/// 初期用パラメータの設定
/// </summary>
void Enemy::SetInitializeData()
{
	// エネミーのグラフィックをメモリにロード＆表示座標を初期化
	enemyGlaphStr = "data/texture/EpicEnemy.png";
	x = 0;
	y = 50;
	life = ENEMY_LIFE;

	// エネミーが顔を歪めているかどうかの変数に『歪めていない』を表すFALSEを代入
	damageFlag = false;

	rightMove = true;
	speed = 3;
	speedUpLifeLimit = 0;
}
