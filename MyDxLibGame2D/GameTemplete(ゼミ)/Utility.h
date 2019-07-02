//----------------------------//
// ユーティリティ（便利）関数群
//----------------------------//

#pragma once

// π
#define PI    3.1415926535897932384626433832795f

// スクリーンサイズ.
#define SCREEN_W 640
#define SCREEN_H 480

#define LIMIT_TIME_COUNT	30		// ゲームの制限時間.
#define STATE_CHANGE_WAIT	500		// ステート切り替えウェイト(ミリ秒).

// 状態.
enum STATE
{
	STATE_TITLE,		// タイトル.
	STATE_GAME,			// ゲーム中.
	STATE_CLEAR,		// クリア.
	STATE_GAMEOVER,		// ゲームオーバー.
	STATE_NUM			// 番兵.
};

// 指定された全文字列の幅を取得.
int GetDrawStringWidthFull(char* str);

// ディグリーをラジアンに変換
float GetRadian(float degree);
