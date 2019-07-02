// プレイヤー
#pragma once

// プロトタイプ宣言
struct GameObject;
struct Enemy;
struct Shot;

// 定数
#define PLAYER_SPEED			3		// プレイヤーのスピード.
#define PLAYER_LIFE				10		// プレイヤーのライフ.

#define PLAYER_SHOT				3		// ショットの最大数.
#define PLAYER_SHOT_POWER		1		// ショットの威力.
#define PLAYER_SHOT_SPEED		5		// 通常ショットのスピード.
#define PLAYER_SHOT_INTERVAL	10		// 通常ショットの発射インターバル.

// プレイヤー.
struct Player
{
	GameObject	obj;
	int		life = 0;
	bool	damageFlag = false;
	int		damageCounter = 0;

	Shot	shot[PLAYER_SHOT];

	// ショットのインターバルを設定するカウンター
	int shotIntervalCount = 0;
};

// 初期化.
void InitPlayer(Player& player);

// アップデート.
void UpdatePlayer(Player& player, Enemy& enemy);

// 描画.
void DrawPlayer(Player& player);
