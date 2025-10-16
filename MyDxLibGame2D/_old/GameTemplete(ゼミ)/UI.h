//-----------------------------------------------------------------------------
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#pragma once

// プロトタイプ宣言
struct Game;

// UIに使う文字列.
#define	TITLE_STR			"シューティング"
#define	GAME_OVER_STR		"ゲームオーバー"
#define	GAME_CLEAR_STR		"ゲームクリア！"
#define	STARTBUTTON_STR		"スペースキーでスタート"
#define	RETURN_TITLE_STR	"スペースキーでタイトルへ"
#define	TIME_STR			"残り："

// UI用位置調整.
#define	TITLE_STR_OFFSET_Y		-100	// 見出し文字のyオフセット.
#define	PRESS_STR_OFFSET_Y		100		// ボタンを押してください系UIのyオフセット.
#define	TIME_STR_OFFSET_X		-100	// 制限時間(「残り」)のxオフセット.
#define	TIME_STR_POS_Y			25		// 残り時間UIのyポジション.
#define	TITLE_FONT_SIZE			64		// 見出し文字のフォントサイズ.
#define	PRESS_STR_FONT_SIZE		24		// ボタンを押してください系UIのサイズ.
#define	TIME_STR_FONT_SIZE		24		// 制限時間(「残り」)のフォントサイズ.
#define	TIME_NUM_STR_FONT_SIZE	40		// 制限時間(数字)のフォントサイズ.
#define	ENEMY_HP_STR_FONT_SIZE	24		// 敵体力のフォントサイズ.
#define	ENEMY_HP_STR_POS_Y		2		// 敵体力文字のyポジション.
#define	ENEMY_HP_STR_POS_X		10		// 敵体力文字のxポジション.
#define	ENEMY_HP_LEFT_OFFSET_X	80		// 敵体力の左からのオフセット.
#define	ENEMY_HP_RIGHT_OSSET_X	30		// 敵体力画像の右からのオフセット.
#define	ENEMY_HP_HIGHT			15		// 敵体力画像の高さ.
#define	PLAYER_HP_FONT_SIZE		10		// プレイヤーの体力サイズ.
#define	PLAYER_HP_OFFSET_Y		45		// プレイヤー体力のオフセット.

// UI
struct UI
{
	int		hpGraph;
	int		hpBackGraph;
};

// UI初期化.
void InitUI(UI& ui);

// UI描画.
void DrawUI(UI& ui, Game& game);