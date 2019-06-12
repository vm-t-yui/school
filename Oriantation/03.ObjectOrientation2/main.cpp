//-----------------------------------------------------------------------------
// @brief  メイン処理.
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#include "DxLib.h"

//-----------------------------------------------------------------------------
// @brief  メイン関数.
//-----------------------------------------------------------------------------
#define PLAYER_SPEED	6		// プレイヤーのスピード.

#define SHOT			3		// ショットの最大数.
#define SHOT_SPEED		20		// 通常ショットのスピード.
#define SHOT_INTERVAL	3		// 通常ショットの発射インターバル.

#define ENEMY_LIFE		30

// スクリーンサイズ.
#define SCREEN_W 1024
#define SCREEN_H 720

//----------------------------//
// 構造体宣言.
//----------------------------//
// プレイヤー.
struct Player
{
	int graph;
	int x;
	int y;
	int w;
	int h;
	int shotIntervalCount;
};

// エネミー.
struct Enemy
{
	int		x;
	int		y;
	int		graph;
	bool	damageFlag;
	int		damageCounter;
	int		damageGraph;
	int		w;
	int		h;
	int		life;
	bool	isRightMove;
};

// ショット.
struct Shot
{
	int		graph;
	int		x;
	int		y;
	bool	visibleFlag;
	int		w;
	int		h;
};
//----------------------------//
// グローバル変数.
//----------------------------//
Player player;
Enemy enemy;
Shot shot[SHOT];


//----------------------------//
// プレイヤー関数群.
//----------------------------//
// 初期化.
void InitPlayer()
{
	// プレイヤーのグラフィックをメモリにロード＆表示座標を初期化
	player.graph = LoadGraph("data/texture/EpicPlayer.png");
	player.x = SCREEN_W/2;
	player.y = SCREEN_H - 100;

	// プレイヤーと弾の画像のサイズを得る
	GetGraphSize(player.graph, &player.w, &player.h);

	player.shotIntervalCount = 0;
}

// アップデート.
void UpdatePlayer()
{
	// 矢印キーを押していたらプレイヤーを移動させる
	if (CheckHitKey(KEY_INPUT_UP) == 1)
	{
		player.y -= PLAYER_SPEED;
	}
	if (CheckHitKey(KEY_INPUT_DOWN) == 1)
	{
		player.y += PLAYER_SPEED;
	}
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
		// 前フレームでショットボタンを押したかが保存されている変数がfalseだったら弾を発射
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
}

// 描画.
void DrawPlayer()
{
	DrawGraph(player.x, player.y, player.graph, TRUE);
}

//----------------------------//
// エネミー関数群.
//----------------------------//
// 初期化.
void InitEnemy()
{
	// エネミーのグラフィックをメモリにロード＆表示座標を初期化
	char* enemyGlaphStr = "data/texture/EpicEnemy.png";
	enemy.graph = LoadGraph(enemyGlaphStr);
	enemy.damageGraph = LoadGraph(enemyGlaphStr);
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
	if (enemy.isRightMove == true)
	{
		enemy.x += 3;
	}
	else
	{
		enemy.x -= 3;
	}

	// エネミーが画面端からでそうになっていたら画面内の座標に戻してあげ、移動する方向も反転する
	if (enemy.x > SCREEN_W - enemy.w)
	{
		enemy.x = SCREEN_W - enemy.w;
		enemy.isRightMove = false;
	}
	else if (enemy.x < 0)
	{
		enemy.x = 0;
		enemy.isRightMove = true;
	}

	// エネミーを描画
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
	int shotGraph;
	shotGraph = LoadGraph("data/texture/SuperShot.png");
	for (int i = 0; i < SHOT; i++)
	{
		shot[i].graph = shotGraph;
	}

	// 弾が画面上に存在しているか保持する変数に『存在していない』を意味するfalseを代入しておく
	for (int i = 0; i < SHOT; i++)
	{
		shot[i].visibleFlag = false;
	}

	// 弾のグラフィックのサイズをえる
	int shotW, shotH;
	GetGraphSize(shotGraph, &shotW, &shotH);
	for (int i = 0; i < SHOT; i++)
	{
		shot[i].w = shotW;
		shot[i].h = shotH;
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

				enemy.life -= 1;
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
	InitPlayer();
	InitEnemy();
	InitShot();

	// ゲームループ.
	while (1)
	{
		// 画面を初期化(真っ黒にする)
		ClearDrawScreen();

		// 各アップデート関数を呼ぶ.
		UpdatePlayer();
		UpdateEnemy();
		UpdateShot();

		// 各描画関数を呼ぶ.
		DrawShot();
		DrawPlayer();
		DrawEnemy();

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