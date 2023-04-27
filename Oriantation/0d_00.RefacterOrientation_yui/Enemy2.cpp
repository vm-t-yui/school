#include "DxLib.h"
#include "Enemy2.h"

/// <summary>
/// 初期用パラメータの設定
/// </summary>
void Enemy2::SetInitializeData()
{
	// エネミーのグラフィックをメモリにロード＆表示座標を初期化
	enemyGlaphStr = "data/texture/EpicEnemy2.png";
	x = 0;
	y = 100;
	life = ENEMY_LIFE2;

	// エネミーが顔を歪めているかどうかの変数に『歪めていない』を表すFALSEを代入
	damageFlag = false;

	rightMove = true;
	speed = 2;
	speedUpLifeLimit = 0;
}
