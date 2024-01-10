//-----------------------------------------------------------------------------
// @brief  2Dアクションサンプルプログラム.
// 2024 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
// 

#include "DxLib.h"

//-----------------------------------------//
// 定数
//-----------------------------------------//
const int ScreenWidth     = 640;                          // 画面の横幅
const int ScreenHeight    = 480;                          // 画面の縦幅
const int MapChipSize     = 32;                           // 一つのチップのサイズ
const int MapWidth        = ScreenWidth / MapChipSize;    // マップの横幅
const int MapHeight       = ScreenHeight / MapChipSize;   // マップの縦幅
const float Gravity       = 0.3f;                         // キャラに掛かる重力加速度
const float JumpPower     = 9.0f;                         // キャラのジャンプ力
const float Speed         = 5.0f;                         // キャラの移動スピード

/// <summary>
/// マップデータ
/// </summary>
const char MapData[MapHeight][MapWidth] =
{
	0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
	1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1,
	1,0,0,1,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1,
	1,0,0,1,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,1,0,1,
	1,0,0,1,1, 1,1,0,0,0, 0,0,0,0,0, 0,0,1,0,1,

	1,0,0,0,0, 0,0,0,1,1, 0,0,0,0,0, 0,0,1,0,1,
	1,0,0,0,0, 0,0,0,0,0, 0,0,1,1,0, 0,0,1,0,1,
	1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,1,0,1,
	1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1,
	1,0,0,0,0, 0,0,1,1,0, 0,0,0,0,0, 1,0,0,0,1,

	1,0,0,0,0, 1,1,1,1,1, 0,0,0,0,1, 1,0,0,0,1,
	1,0,0,0,0, 1,1,1,1,1, 0,0,0,1,1, 1,0,0,0,1,
	1,0,0,0,0, 0,0,0,0,0, 0,0,0,1,1, 1,0,0,0,1,
	1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1,
	1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
};

/// <summary>
/// プレイヤー構造体
/// </summary>
struct Player
{
	VECTOR	pos;		// 座標
	VECTOR	dir;		// 座標
	float	w, h;		// 幅、高さ
	float	fallSpeed;	// プレイヤーの落下速度
	bool	isGround;	// プレイヤーが接地中か
};

//-----------------------------------------//
// プロトタイプ宣言
//-----------------------------------------//
void InitPlayer(Player& player);
void UpdatePlayer(Player& player);
VECTOR CheckPlayerHitWithMap(Player& player, VECTOR velocity);
void DrawPlayer(Player& player);
void DrawMap();

/// <summary>
/// Main
/// </summary>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ChangeWindowMode(TRUE);

	// ＤＸライブラリの初期化
	if (DxLib_Init() == -1) return -1;

	// 描画先を裏画面にセット
	SetDrawScreen(DX_SCREEN_BACK);

	// プレイヤー初期化
	Player player;
	InitPlayer(player);

	// メインループ開始、ＥＳＣキーで外に出る
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		auto prevTime = GetNowHiPerformanceCount();	// 処理が始まる前の時間
		
		ClearDrawScreen();// 画面のクリア

		// 更新
		UpdatePlayer(player);
		DrawPlayer(player);

		// 描画
		//DrawMap();
		
		ScreenFlip();	// 画面の更新

		// 雑なfps固定処理
		// 差を求めて、1回の画面更新が1/60秒になるようにwhileループ回して待つ
		auto afterTime = GetNowHiPerformanceCount(); // 処理が終わった後の時間
		while (afterTime - prevTime < 16667)
		{
			afterTime = GetNowHiPerformanceCount();
		}
	}

	// ＤＸライブラリの後始末
	DxLib_End();

	// 終了
	return 0;
}

/// <summary>
/// プレイヤーの初期化
/// </summary>
void InitPlayer(Player& player)
{
	player.pos = VGet(320.0f, 240.0f, 0);
	player.dir = VGet(0, 0, 0);
	player.w = 20;
	player.h = 60;
	player.fallSpeed = 0.0f;
	player.isGround = false;
}

/// <summary>
/// プレイヤーの更新
/// </summary>
void UpdatePlayer(Player& player)
{
	// 入力状態を更新
	// パッド１とキーボードから入力を得る
	auto input = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	// プレイヤーの移動処理
	// 左右の移動方向を出す
	player.dir = VGet(0,0,0);
	if (input & PAD_INPUT_LEFT)
	{
		player.dir = VAdd(player.dir, VGet(-1, 0, 0));
	}
	if (input & PAD_INPUT_RIGHT)
	{
		player.dir = VAdd(player.dir, VGet(1, 0, 0));
	}

	// 正規化
	if (VSquareSize(player.dir) > 0)
	{
		player.dir = VNorm(player.dir);
	}

	// 移動量を出す
	auto velocity = VScale(player.dir, Speed);

	// 地に足が着いている場合のみジャンプボタン(ボタン１ or Ｚキー)を見る
	if (player.isGround && (input & PAD_INPUT_A))
	{
		player.fallSpeed = -JumpPower;
		player.isGround = false;
	}

	// 落下速度を更新
	player.fallSpeed += Gravity;

	// 落下速度を移動量に加える
	velocity = VAdd(velocity, VGet(0, player.fallSpeed, 0));

	// 当たり判定をして、壁にめり込まないようにvelocityを操作する
	velocity = CheckPlayerHitWithMap(player, velocity);

	// 移動
	player.pos = VAdd(player.pos, velocity);
}

/// <summary>
/// プレイヤーとマップの当たり判定を行い、調整したvelocity（移動ベクトル)を返す
/// </summary>
VECTOR CheckPlayerHitWithMap(Player& player, VECTOR velocity)
{
	////////////////////////////////////////////////////////////////////////
	// TODO:ここで当たり判定をして、壁にめり込まないようにvelocityを操作する
	////////////////////////////////////////////////////////////////////////
	VECTOR ret = velocity;

	// いったん仮で地面に当たっていたらfallSpeedをゼロにする
	auto bottomY = (ScreenHeight - player.h * 0.5f);
	if (player.pos.y + ret.y > bottomY)
	{
		// ちょうど画面端からプレイヤーの身長の半分だけ上げた位置につくように
		ret.y = bottomY - player.pos.y;
		player.isGround = true;	// 接地中
	}

	return ret;
}

/// <summary>
/// プレイヤー描画
/// </summary>
void DrawPlayer(Player& player)
{
	// キャラクタの描画
	DrawBox((int)(player.pos.x - player.w * 0.5f), (int)(player.pos.y - player.h * 0.5f),
		(int)(player.pos.x + player.w * 0.5f) + 1, (int)(player.pos.y + player.h * 0.5f) + 1,
		GetColor(255, 0, 0), TRUE);
}

/// <summary>
/// マップ描画
/// </summary>
void DrawMap()
{
	for (int i = 0; i < MapHeight; i++)
	{
		for (int j = 0; j < MapWidth; j++)
		{
			// １は当たり判定チップを表しているので１のところだけ描画
			if (MapData[i][j] == 1)
			{
				DrawBox(j * MapChipSize, i * MapChipSize,
					j * MapChipSize + MapChipSize, i * MapChipSize + MapChipSize,
					GetColor(255, 255, 255), TRUE);
			}
		}
	}
}