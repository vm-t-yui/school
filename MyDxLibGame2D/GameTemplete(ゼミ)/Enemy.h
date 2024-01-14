//-----------------------------------------------------------------------------
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
// エネミー
#pragma once

// プロトタイプ宣言
struct GameObject;
struct Player;
struct Shot;

// 定数
#define ENEMY_SPEED			3		// 敵のスピード.
#define ENEMY_LIFE			10		// 敵のライフ.

#define ENEMY_SHOT			3		// 敵ショットの最大数.
#define ENEMY_SHOT_POWER	1		// 敵ショットの威力.
#define ENEMY_SHOT_SPEED	5		// 敵ショットのスピード.
#define ENEMY_SHOT_INTERVAL	10		// 敵ショットの発射インターバル.

// エネミー.
struct Enemy
{
	GameObject	obj;
	bool	damageFlag = false;
	int		damageCounter = 0;
	int		damageGraph = -1;
	int		life = 0;

	Shot	shot[ENEMY_SHOT];

	// 敵ショットのインターバルを設定するカウンター
	int shotIntervalCount = 0;

	// 右移動しているかどうかのフラグをリセット
	bool rightMove = true;
};

// 初期化.
void InitEnemy(Enemy& enemy);

// アップデート.
void UpdateEnemy(Enemy& enemy, Player& player);

// 描画.
void DrawEnemy(Enemy& enemy);