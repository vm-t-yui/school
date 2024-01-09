//-----------------------------------------------------------------------------
// @brief  アクションサンプルプログラム.
// 2019 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
// 　２Ｄアクションの基本、マップとの当たり判定です。
// 　画面外に出たら見えないところで永遠に落ちてゆきますのでご注意
// 　実際はキャラクタが真四角ということはないので、いろいろと改良を加える

#include "DxLib.h"

#define SCREEN_WIDTH     (640)                          // 画面の横幅
#define SCREEN_HEIGHT    (480)                          // 画面の縦幅
#define CHIP_SIZE        (32)                           // 一つのチップのサイズ
#define MAP_WIDTH        (SCREEN_WIDTH / CHIP_SIZE)     // マップの横幅
#define MAP_HEIGHT       (SCREEN_HEIGHT / CHIP_SIZE)    // マップの縦幅

#define G                (0.3F)                         // キャラに掛かる重力加速度
#define JUMP_POWER       (9.0f)                         // キャラのジャンプ力
#define SPEED            (5.0f)                         // キャラの移動スピード

// マップデータ
char MapData[MAP_HEIGHT][MAP_WIDTH] =
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

struct Player
{
	float	x, y;		// プレイヤーの座標(中心座標)
	float	fallSpeed;	// プレイヤーの落下速度
	bool	jumpFlag;	// プレイヤーがジャンプ中か、のフラグ
	int		size = 30;	// 画像のサイズ（縦横同じ）
};

Player player;
int input;					// 入力状態

//-----------------------------------------------------------------------------
// プロトタイプ宣言
//-----------------------------------------------------------------------------
int MovePlayer(Player& player, float MoveX, float MoveY);
int MapHitCheck(float X, float Y, float& MoveX, float& MoveY);
int GetChipParam(float X, float Y);

//-----------------------------------------
// WinMain関数
//-----------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ChangeWindowMode(TRUE);

	// ＤＸライブラリの初期化
	if (DxLib_Init() == -1) return -1;

	// 描画先を裏画面にセット
	SetDrawScreen(DX_SCREEN_BACK);

	// 垂直同期信号を待たない
	SetWaitVSyncFlag(FALSE);

	// プレイヤーの座標を初期化
	player.x = 320.0f;
	player.y = 240.0f;

	// プレイヤーの落下速度を初期化
	player.fallSpeed = 0.0f;

	// ジャンプ中フラグを倒す
	player.jumpFlag = FALSE;

	// 入力状態の初期化
	input = 0;

	// メインループ開始、ＥＳＣキーで外に出る
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// 画面のクリア
		ClearDrawScreen();

		// 入力状態を更新
		// パッド１とキーボードから入力を得る
		input = GetJoypadInputState(DX_INPUT_KEY_PAD1);

		// プレイヤーの移動処理
		float MoveX, MoveY;

		// 移動量の初期化
		MoveX = 0.0f;
		MoveY = 0.0f;

		// 左右の移動を見る
		if ((input & PAD_INPUT_LEFT) != 0) MoveX -= SPEED;
		if ((input & PAD_INPUT_RIGHT) != 0) MoveX += SPEED;

		// 地に足が着いている場合のみジャンプボタン(ボタン１ or Ｚキー)を見る
		if (player.jumpFlag == FALSE && (input & PAD_INPUT_A) != 0)
		{
			player.fallSpeed = -JUMP_POWER;
			player.jumpFlag = TRUE;
		}

		// 落下処理
		player.fallSpeed += G;

		// 落下速度を移動量に加える
		MoveY = player.fallSpeed;

		// 移動量に基づいてキャラクタの座標を移動
		MovePlayer(player, MoveX, MoveY);

		// マップの描画
		for (int i = 0; i < MAP_HEIGHT; i++)
		{
			for (int j = 0; j < MAP_WIDTH; j++)
			{
				// １は当たり判定チップを表しているので１のところだけ描画
				if (MapData[i][j] == 1)
				{
					DrawBox(j * CHIP_SIZE, i * CHIP_SIZE,
						j * CHIP_SIZE + CHIP_SIZE, i * CHIP_SIZE + CHIP_SIZE,
						GetColor(255, 255, 255), TRUE);
				}
			}
		}

		// キャラクタの描画
		DrawBox((int)(player.x - player.size * 0.5f), (int)(player.y - player.size * 0.5f),
			(int)(player.x + player.size * 0.5f) + 1, (int)(player.y + player.size * 0.5f) + 1,
			GetColor(255, 0, 0), TRUE);

		// 画面の更新
		ScreenFlip();
	}

	// ＤＸライブラリの後始末
	DxLib_End();

	// 終了
	return 0;
}

//-----------------------------------------
// キャラクタをマップとの当たり判定を考慮しながら移動する
//-----------------------------------------
int MovePlayer(Player& player, float MoveX, float MoveY)
{
	float Dummy = 0.0f;
	float hsize;

	// キャラクタの左上、右上、左下、右下部分が当たり判定のある
	// マップに衝突しているか調べ、衝突していたら補正する

	// 半分のサイズを算出
	hsize = player.size * 0.5f;

	// 先ず上下移動成分だけでチェック。そのために移動量なしのDummy変数を渡しています
	//---------------------------------------------------------------------//
	// MoveX,MoveYは「参照」で渡しています。
	// この関数の中で、ぶつかった場合は移動量を補正して、壁にぶつかる前に止めます
	//---------------------------------------------------------------------//
	// 左下のチェック、もしブロックの上辺に着いていたら落下を止める
	if (MapHitCheck(player.x - hsize, player.y + hsize, Dummy, MoveY) == 3)
	{
		player.fallSpeed = 0.0f;
	}

	// 右下のチェック、もしブロックの上辺に着いていたら落下を止める
	if (MapHitCheck(player.x + hsize, player.y + hsize, Dummy, MoveY) == 3)
	{
		player.fallSpeed = 0.0f;
	}

	// 左上のチェック、もしブロックの下辺に当たっていたら落下させる
	if (MapHitCheck(player.x - hsize, player.y - hsize, Dummy, MoveY) == 4)
	{
		player.fallSpeed *= -1.0f;
	}

	// 右上のチェック、もしブロックの下辺に当たっていたら落下させる
	if (MapHitCheck(player.x + hsize, player.y - hsize, Dummy, MoveY) == 4)
	{
		player.fallSpeed *= -1.0f;
	}

	// 補正された上下移動成分を加算して実際に移動。あたってなかったらそのまま計算される
	player.y += MoveY;

	// 後に左右移動成分だけでチェック
	// 左下のチェック
	MapHitCheck(player.x - hsize, player.y + hsize, MoveX, Dummy);

	// 右下のチェック
	MapHitCheck(player.x + hsize, player.y + hsize, MoveX, Dummy);

	// 左上のチェック
	MapHitCheck(player.x - hsize, player.y - hsize, MoveX, Dummy);

	// 右上のチェック
	MapHitCheck(player.x + hsize, player.y - hsize, MoveX, Dummy);

	// 左右移動成分を加算
	player.x += MoveX;

	// 接地判定
		// キャラクタの左下と右下の下に地面があるか調べる
	if (GetChipParam(player.x - player.size * 0.5f, player.y + player.size * 0.5f + 1.0f) == 0 &&
		GetChipParam(player.x + player.size * 0.5f, player.y + player.size * 0.5f + 1.0f) == 0)
	{
		// 足場が無かったらジャンプ中にする
		player.jumpFlag = TRUE;
	}
	else
	{
		// 足場が在ったら接地中にする
		player.jumpFlag = FALSE;
	}

	// 終了
	return 0;
}


//-----------------------------------------
// マップとの当たり判定
// 戻り値 0:当たらなかった  1:左辺に当たった  2:右辺に当たった 3:上辺に当たった  4:下辺に当たった
// 注意：MoveX と MoveY、どっちか片方が０じゃないとまともに動作しません
// MoveX,MoveYは「参照」で渡しています。
// この関数の中で、ぶつかった場合は移動量を補正して、壁にぶつかる前に止めます
//-----------------------------------------
int MapHitCheck(float X, float Y, float& MoveX, float& MoveY)
{
	float afterX, afterY;

	// 移動量を足す
	afterX = X + MoveX;
	afterY = Y + MoveY;

	// 当たり判定のあるブロックに当たっているかチェック
	if (GetChipParam(afterX, afterY) == 1)
	{
		float blockLeftX, blockTopY, blockRightX, blockBottomY;

		// 当たっていたら壁から離す処理を行う
		// ブロックの上下左右の座標を算出
		blockLeftX = (float)((int)afterX / CHIP_SIZE) * CHIP_SIZE;			// 左辺の X 座標
		blockRightX = (float)((int)afterX / CHIP_SIZE + 1) * CHIP_SIZE;		// 右辺の X 座標

		blockTopY = (float)((int)afterY / CHIP_SIZE) * CHIP_SIZE;			// 上辺の Y 座標
		blockBottomY = (float)((int)afterY / CHIP_SIZE + 1) * CHIP_SIZE;	// 下辺の Y 座標

		// 上辺に当たっていた場合
		if (MoveY > 0.0f)
		{
			// 移動量を補正する（
			MoveY = blockTopY - Y - 1.0f;

			// 上辺に当たったと返す
			return 3;
		}

		// 下辺に当たっていた場合
		if (MoveY < 0.0f)
		{
			// 移動量を補正する
			MoveY = blockBottomY - Y + 1.0f;

			// 下辺に当たったと返す
			return 4;
		}

		// 左辺に当たっていた場合
		if (MoveX > 0.0f)
		{
			// 移動量を補正する
			MoveX = blockLeftX - X - 1.0f;

			// 左辺に当たったと返す
			return 1;
		}

		// 右辺に当たっていた場合
		if (MoveX < 0.0f)
		{
			// 移動量を補正する
			MoveX = blockRightX - X + 1.0f;

			// 右辺に当たったと返す
			return 2;
		}

		// ここに来たら適当な値を返す
		return 4;
	}

	// どこにも当たらなかったと返す
	return 0;
}

//-----------------------------------------
// マップチップの値を取得する関数
//-----------------------------------------
int GetChipParam(float X, float Y)
{
	int x, y;

	// 整数値へ変換
	x = (int)X / CHIP_SIZE;
	y = (int)Y / CHIP_SIZE;

	// マップからはみ出ていたら 0 を返す
	if (x >= MAP_WIDTH || y >= MAP_HEIGHT || x < 0 || y < 0) return 0;

	// 指定の座標に該当するマップの情報を返す
	return MapData[y][x];
}
