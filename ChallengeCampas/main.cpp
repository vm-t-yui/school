﻿//-----------------------------------------------------------------------------
// @brief  メイン処理.
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#include <string>
#include "DxLib.h"

//-----------------------------------------------------------------------------
// @brief  メイン関数.
//-----------------------------------------------------------------------------
#define PLAYER_SPEED		3		// プレイヤーのスピード.
#define PLAYER_LIFE			10		// プレイヤーのライフ.

#define SHOT				3		// ショットの最大数.
#define SHOT_POWER			1		// ショットの威力.
#define SHOT_SPEED			5		// 通常ショットのスピード.
#define SHOT_INTERVAL		10		// 通常ショットの発射インターバル.

#define ENEMY_SPEED			3		// 敵のスピード.
#define ENEMY_LIFE			10		// 敵のライフ.

#define ENEMY_SHOT			3		// 敵ショットの最大数.
#define ENEMY_SHOT_POWER	1		// 敵ショットの威力.
#define ENEMY_SHOT_SPEED	5		// 敵ショットのスピード.
#define ENEMY_SHOT_INTERVAL	10		// 敵ショットの発射インターバル.

#define LIMIT_TIME_COUNT	30		// ゲームの制限時間.

// スクリーンサイズ.
#define SCREEN_W 640
#define SCREEN_H 480

//----------------------------//
// 構造体宣言.
//----------------------------//
// プレイヤー.
struct Player
{
	int		graph = -1;
	int		x = 0;
	int		y = 0;
	int		w = 0;
	int		h = 0;
	int		life = 0;
	bool	damageFlag = false;
	int		damageCounter = 0;

	// ショットのインターバルを設定するカウンター
	int shotIntervalCount = 0;
};

// エネミー.
struct Enemy
{
	int		x = 0;
	int		y = 0;
	int		graph = -1;
	bool	damageFlag = false;
	int		damageCounter = 0;
	int		damageGraph = -1;
	int		w = 0;
	int		h = 0;
	int		life = 0;

	// 敵ショットのインターバルを設定するカウンター
	int shotIntervalCount = 0;

	// 右移動しているかどうかのフラグをリセット
	bool rightMove = true;
};

// ショット.
struct Shot
{
	int		graph = -1;
	int		x = 0;
	int		y = 0;
	bool	visibleFlag = false;
	int		w = 0;
	int		h = 0;
};

// 背景.
struct BG
{
	int			graph = -1;
	VERTEX2D	vert[6];
	int			w = 0;
	int			h = 0;
};
// 背景種別.
enum BG_KIND
{
	BG_KIND_BACK,		// 後方.
	BG_KIND_MIDDLE,		// 中.
	BG_KIND_FRONT,		// 前方.
	BG_KIND_NUM			// 番兵.
};

// 背景の縦スクロールスピード.
static const float BG_V_SPEED[BG_KIND_NUM] =
{
	0.001f,
	0.0015f,
	0.003f
};

// 背景の横スクロールの、プレイヤー位置による反映比率.
static const float BG_U_PLAYERX_FACTOR[BG_KIND_NUM] =
{
	0.1f,
	0.5,
	0.8f
};

// ルール.
// 状態.
enum STATE
{
	STATE_TITLE,		// タイトル.
	STATE_GAME,			// ゲーム中.
	STATE_CLEAR,		// クリア.
	STATE_GAMEOVER,		// ゲームオーバー.
	STATE_NUM			// 番兵.
};
struct Rule
{
	int gameStartTime = 0;
	int state = STATE_TITLE;
	int prevKeyInput = 0;
    bool keyOn = false;
    bool prevKeyOn = false;
    bool keyRelease = false;
};

// ステート切り替えウェイト(ミリ秒).
static const int STATE_CHANGE_WAIT = 500;

// UIに使う文字列.
static const std::string	TITLE_STR			= "シューティング";
static const std::string	GAME_OVER_STR		= "ゲームオーバー";
static const std::string	GAME_CLEAR_STR		= "ゲームクリア！";
static const std::string	STARTBUTTON_STR		= "スペースキーでスタート";
static const std::string	RETURN_TITLE_STR	= "スペースキーでタイトルへ";
static const std::string	TIME_STR			= "残り：";

// UI用位置調整.
static const int	TITLE_STR_OFFSET_Y		= -100;	// 見出し文字のyオフセット.
static const int	PRESS_STR_OFFSET_Y		= 100;	// ボタンを押してください系UIのyオフセット.
static const int	TIME_STR_OFFSET_X		= -100;	// 制限時間(「残り」)のxオフセット.
static const int	TIME_STR_POS_Y			= 25;	// 残り時間UIのyポジション.
static const int	TITLE_FONT_SIZE			= 64;	// 見出し文字のフォントサイズ.
static const int	PRESS_STR_FONT_SIZE		= 24;	// ボタンを押してください系UIのサイズ.
static const int	TIME_STR_FONT_SIZE		= 24;	// 制限時間(「残り」)のフォントサイズ.
static const int	TIME_NUM_STR_FONT_SIZE	= 40;	// 制限時間(数字)のフォントサイズ.
static const int	ENEMY_HP_STR_FONT_SIZE	= 24;	// 敵体力のフォントサイズ.
static const int	ENEMY_HP_STR_POS_Y      = 2;	// 敵体力文字のyポジション.
static const int	ENEMY_HP_STR_POS_X		= 10;	// 敵体力文字のxポジション.
static const int	ENEMY_HP_LEFT_OFFSET_X	= 80;	// 敵体力の左からのオフセット.
static const int	ENEMY_HP_RIGHT_OSSET_X	= 30;	// 敵体力画像の右からのオフセット.
static const int	ENEMY_HP_HIGHT			= 15;	// 敵体力画像の高さ.
static const int	PLAYER_HP_FONT_SIZE		= 10;	// プレイヤーの体力サイズ.
static const int	PLAYER_HP_OFFSET_Y		= 5;	// プレイヤー体力のオフセット.

//----------------------------//
// グローバル変数.
//----------------------------//
Player player;
Enemy enemy;
Shot shot[SHOT];
Shot enemyShot[ENEMY_SHOT];
BG bg[BG_KIND_NUM];
Rule rule;
int hpGraph;
int hpBackGraph;

//----------------------------//
// プレイヤー関数群.
//----------------------------//
// 初期化.
void InitPlayer()
{
	// プレイヤーのグラフィックをメモリにロード＆表示座標を初期化
	if (player.graph < 0)
	{
		player.graph = LoadGraph("data/texture/player.png");
	}
	player.x = SCREEN_W/2;
	player.y = SCREEN_H - 100;
	player.life = PLAYER_LIFE;

	// エネミーが顔を歪めているかどうかの変数に『歪めていない』を表すFALSEを代入
	player.damageFlag = false;

	// プレイヤーと弾の画像のサイズを得る
	GetGraphSize(player.graph, &player.w, &player.h);
}

// アップデート.
void UpdatePlayer()
{
	// 矢印キーを押していたらプレイヤーを移動させる
	if (CheckHitKey(KEY_INPUT_LEFT) == 1)
	{
		player.x -= PLAYER_SPEED;
	}
	if (CheckHitKey(KEY_INPUT_RIGHT) == 1)
	{
		player.x += PLAYER_SPEED;
	}

	// 弾の発射処理
	if (CheckHitKey(KEY_INPUT_SPACE) == 1)
	{
		// ショットの発射インターバルがなくなっていたら撃てる
		if (player.shotIntervalCount == 0)
		{
			// 画面上にでていない弾があるか、弾の数だけ繰り返して調べる
			for (int i = 0; i < SHOT; i++)
			{
				// 弾iが画面上にでていない場合はその弾を画面に出す
				if (shot[i].visibleFlag == false)
				{
					// 弾iの位置をセット、位置はプレイヤーの中心にする
					shot[i].x = (player.w - shot[i].w) / 2 + player.x;
					shot[i].y = (player.h - shot[i].h) / 2 + player.y;

					// 弾iは現時点を持って存在するので、存在状態を保持する変数にtrueを代入する
					shot[i].visibleFlag = true;

					// 一つ弾を出したので弾を出すループから抜けます
					break;
				}
			}

			// インターバル用のカウンターを設定.
			player.shotIntervalCount = SHOT_INTERVAL;
		}
	}

	if (player.shotIntervalCount != 0)
	{
		--player.shotIntervalCount;
	}

	// プレイヤーが画面左端からはみ出そうになっていたら画面内の座標に戻してあげる
	if (player.x < 0)
	{
		player.x = 0;
	}
	if (player.x > SCREEN_W - player.w)
	{
		player.x = SCREEN_W - player.w;
	}
	if (player.y < 0)
	{
		player.y = 0;
	}
	if (player.y > SCREEN_H - player.h)
	{
		player.y = SCREEN_H - player.h;
	}

	// ダメージを受けているかどうかで処理を分岐
	if (player.damageFlag == true)
	{
		player.damageCounter++;

		if (player.damageCounter == 5)
		{
			// 『ダメージをうけていない』を表すFALSEを代入
			player.damageFlag = false;
		}
	}
}

// 描画.
void DrawPlayer()
{
	if (player.damageFlag == true)
	{
		// ダメージを受けている場合は透明＝表示しない
	}
	else
	{
		DrawGraph(player.x, player.y, player.graph, TRUE);
	}
}

//----------------------------//
// エネミー関数群.
//----------------------------//
// 初期化.
void InitEnemy()
{
	// エネミーのグラフィックをメモリにロード＆表示座標を初期化
	char* enemyGlaphStr = "data/texture/enemy.png";
	if (enemy.graph < 0)
	{
		enemy.graph = LoadGraph(enemyGlaphStr);
	}
	if (enemy.damageGraph < 0)
	{
		enemy.damageGraph = LoadGraph(enemyGlaphStr);
	}
	GraphFilter(enemy.damageGraph, DX_GRAPH_FILTER_HSB, 0, 0, 0, 256);
	enemy.x = 0;
	enemy.y = 50;
	enemy.life = ENEMY_LIFE;

	// エネミーが顔を歪めているかどうかの変数に『歪めていない』を表すFALSEを代入
	enemy.damageFlag = false;

	// エネミーのグラフィックのサイズを得る
	GetGraphSize(enemy.graph, &enemy.w, &enemy.h);
}

// アップデート.
void UpdateEnemy()
{
	// エネミーの座標を移動している方向に移動する
	if (enemy.rightMove == true)
	{
		enemy.x += ENEMY_SPEED;
	}
	else
	{
		enemy.x -= ENEMY_SPEED;
	}

	// エネミーが画面端からでそうになっていたら画面内の座標に戻してあげ、移動する方向も反転する
	if (enemy.x > SCREEN_W - enemy.w)
	{
		enemy.x = SCREEN_W - enemy.w;
		enemy.rightMove = false;
	}
	else if (enemy.x < 0)
	{
		enemy.x = 0;
		enemy.rightMove = true;
	}

	// 敵の弾の発射処理
	// ショットの発射インターバルがなくなっていたら撃つ
	if (enemy.shotIntervalCount == 0)
	{
		// 画面上にでていない弾があるか、弾の数だけ繰り返して調べる
		for (int i = 0; i < ENEMY_SHOT; i++)
		{
			// 弾iが画面上にでていない場合はその弾を画面に出す
			if (enemyShot[i].visibleFlag == false)
			{
				// 弾iの位置をセット、位置は敵の中心にする
				enemyShot[i].x = (enemy.w - enemyShot[i].w) / 2 + enemy.x;
				enemyShot[i].y = (enemy.h - enemyShot[i].h) / 2 + enemy.y;

				// 弾iは現時点を持って存在するので、存在状態を保持する変数にtrueを代入する
				enemyShot[i].visibleFlag = true;

				// 一つ弾を出したので弾を出すループから抜けます
				break;
			}
		}

		// インターバル用のカウンターを設定.
		enemy.shotIntervalCount = ENEMY_SHOT_INTERVAL;
	}

	if (enemy.shotIntervalCount != 0)
	{
		--enemy.shotIntervalCount;
	}

	// ダメージを受けているかどうかで処理を分岐
	if (enemy.damageFlag == true)
	{
		enemy.damageCounter++;

		if (enemy.damageCounter == 5)
		{
			// 『ダメージをうけていない』を表すFALSEを代入
			enemy.damageFlag = false;
		}
	}
}
// 描画.
void DrawEnemy()
{
	if (enemy.life > 0)
	{
		// ダメージを受けている場合はダメージ時のグラフィックを描画する
		if (enemy.damageFlag == true)
		{
			DrawGraph(enemy.x, enemy.y, enemy.damageGraph, TRUE);
		}
		else
		{
			DrawGraph(enemy.x, enemy.y, enemy.graph, TRUE);
		}
	}
}

//----------------------------//
// ショット関数群.
//----------------------------//
// 初期化.
void InitShot()
{
	// ショットのグラフィックをメモリにロード.
	int shotGraph = shot[0].graph;
	if (shotGraph < 0)
	{
		shotGraph = LoadGraph("data/texture/shot.png");
	}
	for (int i = 0; i < SHOT; i++)
	{
		shot[i].graph = shotGraph;
	}
	int enemyShotGraph = enemyShot[0].graph;
	if (enemyShotGraph < 0)
	{
		enemyShotGraph = LoadGraph("data/texture/enemyShot.png");
	}
	for (int i = 0; i < ENEMY_SHOT; i++)
	{
		enemyShot[i].graph = enemyShotGraph;
	}

	// 弾が画面上に存在しているか保持する変数に『存在していない』を意味するfalseを代入しておく
	for (int i = 0; i < SHOT; i++)
	{
		shot[i].visibleFlag = false;
	}
	for (int i = 0; i < ENEMY_SHOT; i++)
	{
		enemyShot[i].visibleFlag = false;
	}

	// 弾のグラフィックのサイズをえる
	int shotW, shotH;
	GetGraphSize(shotGraph, &shotW, &shotH);
	for (int i = 0; i < SHOT; i++)
	{
		shot[i].w = shotW;
		shot[i].h = shotH;
	}
	GetGraphSize(enemyShotGraph, &shotW, &shotH);
	for (int i = 0; i < ENEMY_SHOT; i++)
	{
		enemyShot[i].w = shotW;
		enemyShot[i].h = shotH;
	}
}

// アップデート.
void UpdateShot()
{
	for (int i = 0; i < SHOT; i++)
	{
		// 自機の弾iの移動ルーチン( 存在状態を保持している変数の内容がtrue(存在する)の場合のみ行う )
		if (shot[i].visibleFlag == true)
		{
			// 弾iを移動させる
			shot[i].y -= SHOT_SPEED;

			// 画面外に出てしまった場合は存在状態を保持している変数にfalse(存在しない)を代入する
			if (shot[i].y < 0 - shot[i].h)
			{
				shot[i].visibleFlag = false;
			}
		}

		// 弾のあたり判定.
		// 弾iが存在している場合のみ次の処理に映る
		if (shot[i].visibleFlag == 1 && enemy.life > 0)
		{
			// エネミーとの当たり判定
			if (((shot[i].x > enemy.x && shot[i].x < enemy.x + enemy.w) ||
				(enemy.x > shot[i].x && enemy.x < shot[i].x + shot[i].w)) &&
				((shot[i].y > enemy.y && shot[i].y < enemy.y + enemy.h) ||
				(enemy.y > shot[i].y && enemy.y < shot[i].y + shot[i].h)))
			{
				// 接触している場合は当たった弾の存在を消す
				shot[i].visibleFlag = 0;

				// エネミーの顔を歪めているかどうかを保持する変数に『歪めている』を表すTRUEを代入
				enemy.damageFlag = true;

				// エネミーの顔を歪めている時間を測るカウンタ変数に０を代入
				enemy.damageCounter = 0;

				enemy.life -= SHOT_POWER;
			}
		}
	}

	for (int i = 0; i < ENEMY_SHOT; i++)
	{
		// 自機の弾iの移動ルーチン( 存在状態を保持している変数の内容がtrue(存在する)の場合のみ行う )
		if (enemyShot[i].visibleFlag == true)
		{
			// 弾iを移動させる
			enemyShot[i].y += ENEMY_SHOT_SPEED;

			// 画面外に出てしまった場合は存在状態を保持している変数にfalse(存在しない)を代入する
			if (enemyShot[i].y > SCREEN_H)
			{
				enemyShot[i].visibleFlag = false;
			}
		}

		// 弾のあたり判定.
		// 弾iが存在している場合のみ次の処理に映る
		if (enemyShot[i].visibleFlag == 1 && player.life > 0)
		{
			// プレイヤーとの当たり判定
			if (((enemyShot[i].x > player.x && enemyShot[i].x < player.x + player.w) ||
				(player.x > enemyShot[i].x && player.x < enemyShot[i].x + enemyShot[i].w)) &&
				((enemyShot[i].y > player.y && enemyShot[i].y < player.y + player.h) ||
				(player.y > enemyShot[i].y && player.y < enemyShot[i].y + enemyShot[i].h)))
			{
				// 接触している場合は当たった弾の存在を消す
				enemyShot[i].visibleFlag = 0;

				player.damageFlag = true;
				player.damageCounter = 0;
				player.life -= ENEMY_SHOT_POWER;
			}
		}
	}
}
// 描画.
void DrawShot()
{
	for (int i = 0; i < SHOT; i++)
	{
		if (shot[i].visibleFlag == true)
		{
			// 画面に弾iを描画する
			DrawGraph(shot[i].x, shot[i].y, shot[i].graph, TRUE);
		}
	}
	for (int i = 0; i < ENEMY_SHOT; i++)
	{
		if (enemyShot[i].visibleFlag == true)
		{
			// 画面に弾iを描画する
			DrawGraph(enemyShot[i].x, enemyShot[i].y, enemyShot[i].graph, TRUE);
		}
	}
}

//----------------------------//
// 背景関数群.
//----------------------------//
// 初期化.
void InitBG()
{
	// グラフィックをロードしてサイズを取得.
	if (bg[BG_KIND_FRONT].graph < 0)
	{
		bg[BG_KIND_FRONT].graph = LoadGraph("data/texture/bgFront.png");
	}
	if (bg[BG_KIND_MIDDLE].graph < 0)
	{
		bg[BG_KIND_MIDDLE].graph = LoadGraph("data/texture/bgMiddle.png");
	}
	if (bg[BG_KIND_BACK].graph < 0)
	{
		bg[BG_KIND_BACK].graph = LoadGraph("data/texture/bgBack.png");
	}
	for (int i = 0; i < BG_KIND_NUM; i++)
	{
		GetGraphSize(bg[i].graph, &bg[i].w, &bg[i].h);
	}

	// ２ポリゴン分の頂点のデータをセットアップ
	for (int i = 0; i < BG_KIND_NUM; i++)
	{
		bg[i].vert[0].pos = VGet(0, 0, 0.0f);
		bg[i].vert[0].u = 0.0f;
		bg[i].vert[0].v = 0.0f;

		bg[i].vert[1].pos = VGet(SCREEN_W, 0, 0.0f);
		bg[i].vert[1].u = 1.0f;
		bg[i].vert[1].v = 0.0f;

		bg[i].vert[2].pos = VGet(0, SCREEN_H, 0.0f);
		bg[i].vert[2].u = 0.0f;
		bg[i].vert[2].v = 1.0f;

		bg[i].vert[3].pos = VGet(SCREEN_W, SCREEN_H, 0.0f);
		bg[i].vert[3].u = 1.0f;
		bg[i].vert[3].v = 1.0f;

		bg[i].vert[4].pos = VGet(0, SCREEN_H, 0.0f);
		bg[i].vert[4].u = 0.0f;
		bg[i].vert[4].v = 1.0f;

		bg[i].vert[5].pos = VGet(SCREEN_W, 0, 0.0f);
		bg[i].vert[5].u = 1.0f;
		bg[i].vert[5].v = 0.0f;
		for (int j = 0; j < 6; j++)
		{
			bg[i].vert[j].rhw = 1.0f;
			bg[i].vert[j].dif = GetColorU8(255, 255, 255, 255);
		}
	}
}

// アップデート.
void UpdateBG()
{
	// 頂点のデータを更新
	for (int i = 0; i < BG_KIND_NUM; i++)
	{
		// 縦スクロール.
		for (int j = 0; j < 6; j++)
		{
			bg[i].vert[j].v -= BG_V_SPEED[i];
		}

		// 横スクロール.
		float uOffset = (float)player.x / SCREEN_W * BG_U_PLAYERX_FACTOR[i] + 0.5f;
		bg[i].vert[0].u = 0.0f + uOffset;
		bg[i].vert[1].u = 1.0f + uOffset;
		bg[i].vert[2].u = 0.0f + uOffset;
		bg[i].vert[3].u = 1.0f + uOffset;
		bg[i].vert[4].u = 0.0f + uOffset;
		bg[i].vert[5].u = 1.0f + uOffset;
	}
}

// 描画.
void DrawBG()
{
	for (int i = 0; i < BG_KIND_NUM; i++)
	{
		DrawPolygon2D(bg[i].vert, 2, bg[i].graph, TRUE);
	}
}

//----------------------------//
// UI関数群.
//----------------------------//
// 文字列幅取得(std).
int GetDrawStringWidthStd(const std::string& str)
{
	return GetDrawStringWidth(str.c_str(), (int)strlen(str.c_str()));
}

// UI初期化.
void InitUI()
{
	hpGraph = LoadGraph("data/texture/hp.png");
	hpBackGraph = LoadGraph("data/texture/hpBack.png");
}

// UI描画.
void DrawUI()
{
	std::string timeNumStr;
	std::string lifeStr;

	int hpGraphStartX;
	switch (rule.state)
	{
		// タイトル.
	case STATE_TITLE:
		SetFontSize(TITLE_FONT_SIZE);
		DrawString(SCREEN_W / 2 - GetDrawStringWidthStd(TITLE_STR) / 2, SCREEN_H / 2 + TITLE_STR_OFFSET_Y, TITLE_STR.c_str(), GetColor(255, 255, 255));
		SetFontSize(PRESS_STR_FONT_SIZE);
		DrawString(SCREEN_W / 2 - GetDrawStringWidthStd(STARTBUTTON_STR) / 2, SCREEN_H / 2 + PRESS_STR_OFFSET_Y, STARTBUTTON_STR.c_str(), GetColor(255, 255, 255));
		break;
		// ゲーム中.
	case STATE_GAME:

		// 制限時間.
		SetFontSize(TIME_STR_FONT_SIZE);
		DrawString(SCREEN_W / 2 + TIME_STR_OFFSET_X, TIME_STR_POS_Y, TIME_STR.c_str(), GetColor(255, 0, 0));
		timeNumStr += std::to_string(LIMIT_TIME_COUNT + ((rule.gameStartTime - GetNowCount()) / 1000));
		SetFontSize(TIME_NUM_STR_FONT_SIZE);
		DrawString(SCREEN_W / 2 - GetDrawStringWidthStd(timeNumStr) / 2, TIME_STR_POS_Y, timeNumStr.c_str(), GetColor(255, 0, 0));

		// プレイヤー体力.
		lifeStr = "HP:";
		lifeStr += std::to_string(player.life);
		SetFontSize(TIME_STR_FONT_SIZE);
		DrawString(player.x + player.w / 2 - GetDrawStringWidthStd(lifeStr) / 2, player.y + player.h + PLAYER_HP_OFFSET_Y, lifeStr.c_str(), GetColor(255, 0, 0));

		// 敵体力.
		SetFontSize(ENEMY_HP_STR_FONT_SIZE);
		DrawString(ENEMY_HP_STR_POS_X, ENEMY_HP_STR_POS_Y, "Enemy", GetColor(255, 0, 0));
		hpGraphStartX = ENEMY_HP_STR_POS_X + ENEMY_HP_LEFT_OFFSET_X;

		DrawExtendGraph(
			hpGraphStartX,
			ENEMY_HP_STR_POS_Y,
			SCREEN_W - ENEMY_HP_RIGHT_OSSET_X,
			ENEMY_HP_STR_POS_Y + ENEMY_HP_HIGHT,
			hpBackGraph,
			TRUE);

		DrawExtendGraph(
			hpGraphStartX,
			ENEMY_HP_STR_POS_Y,
			hpGraphStartX + (int)((SCREEN_W - hpGraphStartX - ENEMY_HP_RIGHT_OSSET_X) * ((float)enemy.life / ENEMY_LIFE)),
			ENEMY_HP_STR_POS_Y + ENEMY_HP_HIGHT,
			hpGraph,
			TRUE);

		break;

		// クリア画面.
	case STATE_CLEAR:
		SetFontSize(TITLE_FONT_SIZE);
		DrawString(SCREEN_W / 2 - GetDrawStringWidthStd(GAME_CLEAR_STR) / 2, SCREEN_H / 2 + TITLE_STR_OFFSET_Y, GAME_CLEAR_STR.c_str(), GetColor(255, 255, 0));
		SetFontSize(PRESS_STR_FONT_SIZE);
		DrawString(SCREEN_W / 2 - GetDrawStringWidthStd(RETURN_TITLE_STR) / 2, SCREEN_H / 2 + PRESS_STR_OFFSET_Y, RETURN_TITLE_STR.c_str(), GetColor(255, 255, 255));
		break;
		// ゲームオーバー.
	case STATE_GAMEOVER:
		SetFontSize(TITLE_FONT_SIZE);
		DrawString(SCREEN_W / 2 - GetDrawStringWidthStd(GAME_OVER_STR) / 2, SCREEN_H / 2 + TITLE_STR_OFFSET_Y, GAME_OVER_STR.c_str(), GetColor(255, 0, 0));
		SetFontSize(PRESS_STR_FONT_SIZE);
		DrawString(SCREEN_W / 2 - GetDrawStringWidthStd(RETURN_TITLE_STR) / 2, SCREEN_H / 2 + PRESS_STR_OFFSET_Y, RETURN_TITLE_STR.c_str(), GetColor(255, 255, 255));
		break;
	default:
		break;
	}
}

//----------------------------//
// ルール関数群.
//----------------------------//
// 初期化.
void InitRule()
{
	rule.gameStartTime = 0;
	rule.state = STATE_TITLE;
}

// ステート切り替え.
void ChangeState(int state)
{
	// 即座に切り替わりすぎるので、ちょっと時間を止める.
	WaitTimer(STATE_CHANGE_WAIT);

	// ステートが切り替わった瞬間はキー離した判定をリセット.
	rule.keyOn = false;
	rule.keyRelease = false;

	rule.state = state;
	switch (rule.state)
	{
		// タイトル.
	case STATE_TITLE:
		break;
		// ゲーム中.
	case STATE_GAME:
		rule.gameStartTime = GetNowCount();
		InitPlayer();
		InitShot();
		InitEnemy();
		InitBG();
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
void UpdateRule()
{
    // キー離した瞬間を取る.
    if (rule.keyOn)
    {
        if (CheckHitKey(KEY_INPUT_SPACE) == 0)
        {
            rule.keyOn = false;
            rule.keyRelease = true;
        }
    }
    else if (rule.prevKeyOn == false && CheckHitKey(KEY_INPUT_SPACE) == 1)
    {
        rule.keyRelease = false;
        rule.keyOn = true;
    }
    if (CheckHitKey(KEY_INPUT_SPACE) == 1)
    {
        rule.prevKeyOn = true;
    }
    else
    {
        rule.prevKeyOn = false;
    }

	switch (rule.state)
	{
		// タイトル.
	case STATE_TITLE:
		if (rule.keyRelease)
		{
			ChangeState(STATE_GAME);
		}
		break;
		// ゲーム中.
	case STATE_GAME:
		if (enemy.life <= 0)
		{
			ChangeState(STATE_CLEAR);
		}
		else if (GetNowCount() - rule.gameStartTime > LIMIT_TIME_COUNT * 1000 || player.life <= 0)
		{
			ChangeState(STATE_GAMEOVER);
		}
		break;
		// クリア画面.
	case STATE_CLEAR:
		if (rule.keyRelease)
		{
			ChangeState(STATE_TITLE);
		}
		break;
		// ゲームオーバー.
	case STATE_GAMEOVER:
		if (rule.keyRelease)
		{
			ChangeState(STATE_TITLE);
		}
		break;
	default:
		break;
	}
}

//----------------------------//
// WinMain関数.
//----------------------------//
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	// 画面モードの設定
	SetGraphMode(SCREEN_W, SCREEN_H, 16);		// 解像度をSCREEN_W*SCREEN_H、colorを16bitに設定.
	ChangeWindowMode(TRUE);			// ウインドウモードに.
	
	// ＤＸライブラリ初期化処理
	if (DxLib_Init() == -1)
	{
		return -1;
	}

	// 背景のスクロールをするためにテクスチャモードをラッピングに修正.
	SetTextureAddressMode(DX_TEXADDRESS_WRAP);

	// グラフィックの描画先を裏画面にセット
	SetDrawScreen(DX_SCREEN_BACK);

	// 各初期化を呼ぶ.
	InitRule();
	InitPlayer();
	InitEnemy();
	InitShot();
	InitBG();
	InitUI();

	// ゲームループ.
	while (1)
	{
		// 画面を初期化(真っ黒にする)
		ClearDrawScreen();

		// 各アップデート関数を呼ぶ.
		if (rule.state == STATE_GAME)
		{
			// ルール以外はゲーム中だけupdate.
			UpdatePlayer();
			UpdateEnemy();
			UpdateShot();
			UpdateBG();
		}
		UpdateRule();

		// 各描画関数を呼ぶ.
		if (rule.state == STATE_GAME)
		{
			// UI以外はゲーム中だけupdate.
			DrawBG();
			DrawShot();
			DrawPlayer();
			DrawEnemy();
		}
		DrawUI();

		// 裏画面の内容を表画面にコピーする（描画の確定）.
		ScreenFlip();

		// Windows 特有の面倒な処理をＤＸライブラリにやらせる
		// マイナスの値（エラー値）が返ってきたらループを抜ける
		if (ProcessMessage() < 0)
		{
			break;
		}
		// もしＥＳＣキーが押されていたらループから抜ける
		else if (CheckHitKey(KEY_INPUT_ESCAPE))
		{
			break;
		}
	}

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;					// ソフトの終了
}