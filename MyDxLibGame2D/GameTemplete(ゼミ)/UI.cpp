#include "Utility.h"
#include "UI.h"
#include "DxLib.h"
#include "GameObject.h"
#include "Shot.h"
#include "Player.h"
#include "Enemy.h"
#include "BG.h"
#include "Game.h"

// UI初期化.
void InitUI(UI& ui)
{
	if (ui.hpGraph < 0)
	{
		ui.hpGraph = LoadGraph("data/texture/hp.png");
	}
	if (ui.hpBackGraph < 0)
	{
		ui.hpBackGraph = LoadGraph("data/texture/hpBack.png");
	}
}

// UI描画.
// ※難しければUI自体を非表示にして自分で考えること！.
void DrawUI(UI& ui, Game& game)
{
	// 時間を入れる文字列.
	char timeNumStr[256];

	// 残りライフを入れる文字列.
	char lifeStr[256];

	// HP表示を始めるX座標.
	int hpGraphStartX;

	// ルールごとに表示物を変える.
	switch (game.state)
	{
		// タイトル.
	case STATE_TITLE:
		SetFontSize(TITLE_FONT_SIZE);

		// 画面サイズ/2 から　描画する文字列の幅2/、つまり画面の中央に文字を表示している.
		DrawString(SCREEN_W / 2 - GetDrawStringWidthFull(TITLE_STR) / 2, SCREEN_H / 2 + TITLE_STR_OFFSET_Y, TITLE_STR, GetColor(255, 255, 255));

		SetFontSize(PRESS_STR_FONT_SIZE);
		DrawString(SCREEN_W / 2 - GetDrawStringWidthFull(STARTBUTTON_STR) / 2, SCREEN_H / 2 + PRESS_STR_OFFSET_Y, STARTBUTTON_STR, GetColor(255, 255, 255));
		break;
		// ゲーム中.
	case STATE_GAME:

		// 制限時間.
		SetFontSize(TIME_STR_FONT_SIZE);
		DrawString(SCREEN_W / 2 + TIME_STR_OFFSET_X, TIME_STR_POS_Y, TIME_STR, GetColor(255, 0, 0));

		// GetNowCountは1000分の１秒なのでそこから計算する.
		sprintf_s(timeNumStr, "%d", LIMIT_TIME_COUNT + ((game.gameStartTime - GetNowCount()) / 1000));

		SetFontSize(TIME_NUM_STR_FONT_SIZE);
		DrawString(SCREEN_W / 2 - GetDrawStringWidthFull(timeNumStr) / 2, TIME_STR_POS_Y, timeNumStr, GetColor(255, 0, 0));

		// プレイヤー体力.
		sprintf_s(lifeStr, "HP:%d", game.player.life);
		SetFontSize(TIME_STR_FONT_SIZE);
		DrawString((int)(game.player.obj.pos.x - GetDrawStringWidthFull(lifeStr) / 2), (int)(game.player.obj.pos.y + PLAYER_HP_OFFSET_Y), lifeStr, GetColor(255, 0, 0));

		// 敵体力.
		SetFontSize(ENEMY_HP_STR_FONT_SIZE);
		DrawString(ENEMY_HP_STR_POS_X, ENEMY_HP_STR_POS_Y, "Enemy", GetColor(255, 0, 0));
		hpGraphStartX = ENEMY_HP_STR_POS_X + ENEMY_HP_LEFT_OFFSET_X;

		// 体力画像は一色の画像を、引き伸ばして画面からいい感じの位置においている.
		DrawExtendGraph(
			hpGraphStartX,
			ENEMY_HP_STR_POS_Y,
			SCREEN_W - ENEMY_HP_RIGHT_OSSET_X,
			ENEMY_HP_STR_POS_Y + ENEMY_HP_HIGHT,
			ui.hpBackGraph,
			TRUE);

		DrawExtendGraph(
			hpGraphStartX,
			ENEMY_HP_STR_POS_Y,
			hpGraphStartX + (int)((SCREEN_W - hpGraphStartX - ENEMY_HP_RIGHT_OSSET_X) * ((float)game.enemy.life / ENEMY_LIFE)),
			ENEMY_HP_STR_POS_Y + ENEMY_HP_HIGHT,
			ui.hpGraph,
			TRUE);

		break;

		// クリア画面.
	case STATE_CLEAR:
		SetFontSize(TITLE_FONT_SIZE);
		DrawString(SCREEN_W / 2 - GetDrawStringWidthFull(GAME_CLEAR_STR) / 2, SCREEN_H / 2 + TITLE_STR_OFFSET_Y, GAME_CLEAR_STR, GetColor(255, 255, 0));
		SetFontSize(PRESS_STR_FONT_SIZE);
		DrawString(SCREEN_W / 2 - GetDrawStringWidthFull(RETURN_TITLE_STR) / 2, SCREEN_H / 2 + PRESS_STR_OFFSET_Y, RETURN_TITLE_STR, GetColor(255, 255, 255));
		break;
		// ゲームオーバー.
	case STATE_GAMEOVER:
		SetFontSize(TITLE_FONT_SIZE);
		DrawString(SCREEN_W / 2 - GetDrawStringWidthFull(GAME_OVER_STR) / 2, SCREEN_H / 2 + TITLE_STR_OFFSET_Y, GAME_OVER_STR, GetColor(255, 0, 0));
		SetFontSize(PRESS_STR_FONT_SIZE);
		DrawString(SCREEN_W / 2 - GetDrawStringWidthFull(RETURN_TITLE_STR) / 2, SCREEN_H / 2 + PRESS_STR_OFFSET_Y, RETURN_TITLE_STR, GetColor(255, 255, 255));
		break;
	default:
		break;
	}
}
