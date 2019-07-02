#pragma once

#define	BG_NUM				3		// 背景の個数.

// プロトタイプ宣言
enum STATE;
struct Player;
struct Enemy;
struct BG;
struct UI;

// ゲーム構造体
struct Game
{
	Player	player;
	Enemy	enemy;
	BG		bg[BG_NUM];
	UI		ui;

	int gameStartTime = 0;
	STATE state;
	int prevKeyInput = 0;
	bool keyOn = false;
	bool prevKeyOn = false;
	bool keyRelease = false;
};

//----------------------------//
// ゲームそのものの関数群.
//----------------------------//
// 初期化.
void InitGame(Game& game);

// ゲームスタート時の初期化
void InitGameStart(Game& game);

// ステート切り替え.
void ChangeGameState(STATE state, Game& game);

// アップデート.
void UpdateGame(Game& game);

// ゲーム描画
void DrawGame(Game& game);