//-----------------------------------------------------------------------------
// @brief  メイン処理.
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#include "DxLib.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

// プレイヤー構造体.
struct Player
{
	int		graph;
	float	x;
	float	y;
	float	speed;
	float	w;
	float	h;
};

// エネミー構造体.
struct Enemy
{
	float	x;
	float	y;
	int		graph;
	float	w;
	float	h;
};

// プロトタイプ宣言.
void InitPlayer(Player& player);
void UpdatePlayer(Player& player);
void DrawPlayer(Player& player);
void InitEnemy(Enemy& enemy);
void UpdateEnemy(Enemy& enemy);
void DrawEnemy(Enemy& enemy);

// WinMain関数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	// 画面モードの設定
	SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, 16);		// 解像度を640*480、colorを16bitに設定.
	ChangeWindowMode(TRUE);			// ウインドウモードに.
	
	// ＤＸライブラリ初期化処理
	if (DxLib_Init() == -1)
	{
		return -1;
	}

	// グラフィックの描画先を裏画面にセット
	SetDrawScreen(DX_SCREEN_BACK);

	// プレイヤー初期化
	Player player;
	InitPlayer(player);

	// エネミー初期化.
	Enemy enemy;
	InitEnemy(enemy);

	// ゲームループ.
	while (1)
	{
		// 画面を初期化(真っ黒にする)
		ClearDrawScreen();

		UpdatePlayer(player);
		UpdateEnemy(enemy);
		DrawPlayer(player);
		DrawEnemy(enemy);

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

//------------------------------//
// プレイヤー初期化.
//------------------------------//
void InitPlayer(Player& player)
{
	// プレイヤーのグラフィックをメモリにロード＆表示座標を初期化
	player.graph = LoadGraph("data/texture/player.png");
	player.x = 288;
	player.y = 400;
	player.speed = 3;

	// プレイヤーの画像のサイズを得る
	int w, h;
	GetGraphSize(player.graph, &w, &h);
	player.w = (float)w;
	player.h = (float)h;
}

//------------------------------//
// プレイヤー更新.
//------------------------------//
void UpdatePlayer(Player& player)
{
	// 矢印キーを押していたらプレイヤーを移動させる
	if (CheckHitKey(KEY_INPUT_UP) == 1)
	{
		player.y -= player.speed;
	}
	if (CheckHitKey(KEY_INPUT_DOWN) == 1)
	{
		player.y += player.speed;
	}
	if (CheckHitKey(KEY_INPUT_LEFT) == 1)
	{
		player.x -= player.speed;
	}
	if (CheckHitKey(KEY_INPUT_RIGHT) == 1)
	{
		player.x += player.speed;
	}

	// プレイヤーが画面端からはみ出そうになっていたら画面内の座標に戻してあげる
	if (player.x < 0)
	{
		player.x = 0;
	}
	if (player.x > SCREEN_WIDTH - player.w)
	{
		player.x = SCREEN_WIDTH - player.w;
	}
	if (player.y < 0)
	{
		player.y = 0;
	}
	if (player.y > SCREEN_HEIGHT - player.h)
	{
		player.y = SCREEN_HEIGHT - player.h;
	}
}

//------------------------------//
// プレイヤーを描画.
//------------------------------//
void DrawPlayer(Player& player)
{
	DrawGraph((int)player.x, (int)player.y, player.graph, TRUE);
}

//------------------------------//
// エネミーの初期化.
//------------------------------//
void InitEnemy(Enemy& enemy)
{
	// エネミーのグラフィックをメモリにロード＆表示座標を初期化
	enemy.graph = LoadGraph("data/texture/enemy.png");
	enemy.x = 0;
	enemy.y = 50;

	// エネミーのグラフィックのサイズを得る
	int w, h;
	GetGraphSize(enemy.graph, &w, &h);
	enemy.w = (float)w;
	enemy.h = (float)h;
}

//------------------------------//
// エネミーの更新.
//------------------------------//
void UpdateEnemy(Enemy& enemy)
{
	++enemy.x;

	if (enemy.x < 0)
	{
		enemy.x = 0;
	}
	if (enemy.x > SCREEN_WIDTH - enemy.w)
	{
		enemy.x = SCREEN_WIDTH - enemy.w;
	}
	if (enemy.y < 0)
	{
		enemy.y = 0;
	}
	if (enemy.y > SCREEN_HEIGHT - enemy.h)
	{
		enemy.y = SCREEN_HEIGHT - enemy.h;
	}
}

//------------------------------//
// エネミーの描画.
//------------------------------//
void DrawEnemy(Enemy& enemy)
{
	DrawGraph((int)enemy.x, (int)enemy.y, enemy.graph, TRUE);
}