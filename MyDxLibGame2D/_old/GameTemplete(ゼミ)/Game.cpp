//-----------------------------------------------------------------------------
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#include "DxLib.h"
#include "Utility.h"
#include "GameObject.h"
#include "Shot.h"
#include "BG.h"
#include "UI.h"
#include "Player.h"
#include "Enemy.h"
#include "Game.h"

//----------------------------//
// ゲームそのものの関数群.
//----------------------------//
// 初期化.
void InitGame(Game& game)
{
	game.gameStartTime = 0;
	game.state = STATE_TITLE;
}

// ゲームスタート時の初期化
void InitGameStart(Game& game)
{
	game.gameStartTime = GetNowCount();

	InitUI(game.ui);
	InitPlayer(game.player);
	InitEnemy(game.enemy);

	// グラフィックをロードしてサイズを取得.
	int bgGraph = game.bg[0].obj.graph;
	if (bgGraph < 0)
	{
		// 注意！画面のサイズと背景画像のサイズは一緒じゃないとダメ
		bgGraph = LoadGraph("data/texture/FancyBG_back.png");
	}
	for (int i = 0; i < BG_NUM; i++)
	{
		InitBG(game.bg[i], bgGraph, i);
	}
}

// ステート切り替え.
void ChangeGameState(STATE state, Game& game)
{
	// 即座に切り替わりすぎるので、ちょっと時間を止める.
	WaitTimer(STATE_CHANGE_WAIT);

	// ステートが切り替わった瞬間はキー離した判定をリセット.
	game.keyOn = false;
	game.keyRelease = false;

	game.state = state;

	// ステートが切り替わった瞬間、必要なら初期化などの処理を行う
	switch (game.state)
	{
		// タイトル.
	case STATE_TITLE:
		break;
		// ゲーム中.
	case STATE_GAME:
		InitGameStart(game);
		break;
		// クリア画面.
	case STATE_CLEAR:
		break;
		// ゲームオーバー.
	case STATE_GAMEOVER:
		break;
	default:
		break;
	}
}

// アップデート.
void UpdateGame(Game& game)
{
	// キー離した瞬間を取る.
	if (game.keyOn)
	{
		if (CheckHitKey(KEY_INPUT_SPACE) == 0)
		{
			game.keyOn = false;
			game.keyRelease = true;
		}
	}
	else if (game.prevKeyOn == false && CheckHitKey(KEY_INPUT_SPACE) == 1)
	{
		game.keyRelease = false;
		game.keyOn = true;
	}
	if (CheckHitKey(KEY_INPUT_SPACE) == 1)
	{
		game.prevKeyOn = true;
	}
	else
	{
		game.prevKeyOn = false;
	}

	// ステートごとに処理をわける.
	switch (game.state)
	{
		// タイトル.
	case STATE_TITLE:
		// キー入力があったらゲーム開始の状態に
		if (game.keyRelease)
		{
			ChangeGameState(STATE_GAME, game);
		}
		break;
		// ゲーム中.
	case STATE_GAME:
		UpdatePlayer(game.player, game.enemy);
		UpdateEnemy(game.enemy, game.player);
		for (int i = 0; i < BG_NUM; i++)
		{
			UpdateBG(game.bg[i]);
		}

		// 敵が死んだらクリア状態に
		if (game.enemy.life <= 0)
		{
			ChangeGameState(STATE_CLEAR, game);
		}
		// プレイヤーが死ぬか時間切れでゲームオーバー
		else if (GetNowCount() - game.gameStartTime > LIMIT_TIME_COUNT * 1000 || game.player.life <= 0)
		{
			ChangeGameState(STATE_GAMEOVER, game);
		}
		break;
		// クリア画面.
	case STATE_CLEAR:
		// キーが押されたらタイトルへ
		if (game.keyRelease)
		{
			ChangeGameState(STATE_TITLE, game);
		}
		break;
		// ゲームオーバー.
	case STATE_GAMEOVER:
		// キーが押されたらタイトルへ
		if (game.keyRelease)
		{
			ChangeGameState(STATE_TITLE, game);
		}
		break;
	default:
		break;
	}
}

// ゲーム描画
void DrawGame(Game& game)
{
	// 各描画関数を呼ぶ.
	// UI以外はゲーム中だけ描画する
	if (game.state == STATE_GAME)
	{
		for (int i = 0; i < BG_NUM; i++)
		{
			DrawBG(game.bg[i]);
		}
		DrawPlayer(game.player);
		DrawEnemy(game.enemy);
	}
	DrawUI(game.ui, game);
}
