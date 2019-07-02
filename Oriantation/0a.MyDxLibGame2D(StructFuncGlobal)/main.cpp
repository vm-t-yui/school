//-----------------------------------------------------------------------------
// @brief  メイン処理.
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#include "DxLib.h"

// プレイヤー構造体
struct Player
{
	VECTOR	pos;
	int		graph;
};

// エネミー構造体
struct Enemy
{
	VECTOR	pos;
	int		graph;
	bool	rightMove;	// エネミーが右移動しているかどうかのフラグ
};

// ショット構造体
struct Shot
{
	int		lifeCounter;	// ショットの生きている時間カウンタ
	VECTOR	dir;
	int		graph;
	VECTOR	pos;
	bool	isAlive;		// 弾が画面上に存在しているか
};

// 変数群
Player player;
Enemy enemy;
Shot shot;

// プレイヤーの初期化
void InitPlayer()
{
	// プレイヤーのグラフィックをメモリにロード＆表示座標を初期化
	player.graph = LoadGraph("data/texture/player.png");
	player.pos.x = 288;
	player.pos.y = 400;
}

// プレイヤーの更新
void UpdatePlayer()
{
	// 矢印キーを押していたらプレイヤーを移動させる
	VECTOR direction = VGet(0, 0, 0);	// direction = (0,0,0)
	float playerSpeed = 5.0f;
	if (CheckHitKey(KEY_INPUT_UP) == 1)
	{
		direction = VAdd(direction, VGet(0, -1, 0)); // direction += (0,-1,0) 
	}
	if (CheckHitKey(KEY_INPUT_DOWN) == 1)
	{
		direction = VAdd(direction, VGet(0, 1, 0)); // direction += (0,1,0) 
	}
	if (CheckHitKey(KEY_INPUT_LEFT) == 1)
	{
		direction = VAdd(direction, VGet(-1, 0, 0)); // direction += (-1,0,0) 
	}
	if (CheckHitKey(KEY_INPUT_RIGHT) == 1)
	{
		direction = VAdd(direction, VGet(1, 0, 0)); // direction += (1,0,0) 
	}
	if (VSize(direction) > 0)						// 長さがゼロより大きい時だけ
	{
		direction = VNorm(direction);				// directionの長さを１に正規化
	}

	// 長さが１になったdirectionにspeedをかける
	VECTOR velocity = VScale(direction, playerSpeed);	// velocity = direction * playerSpeed

	player.pos = VAdd(player.pos, velocity); // player.pos = player.pos + velociity

	// 弾の発射処理
	if (CheckHitKey(KEY_INPUT_SPACE))
	{
		// 弾iが画面上にでていない場合はその弾を画面に出す
		if (shot.isAlive == false)
		{
			int Bw, Bh, Sw, Sh;

			// プレイヤーと弾の画像のサイズを得る
			GetGraphSize(player.graph, &Bw, &Bh);
			GetGraphSize(shot.graph, &Sw, &Sh);

			// 弾iの位置をセット、位置はプレイヤーの中心にする
			shot.pos.x = (Bw - Sw) / 2 + player.pos.x;
			shot.pos.y = (Bh - Sh) / 2 + player.pos.y;

			// 弾iは現時点を持って存在するので、存在状態を保持する変数に１を代入する
			shot.isAlive = true;
			shot.lifeCounter = 0;
		}
	}

	// プレイヤーが画面左端からはみ出そうになっていたら画面内の座標に戻してあげる
	if (player.pos.x < 0)
	{
		player.pos.x = 0;
	}
	if (player.pos.x > 640 - 64)
	{
		player.pos.x = 640 - 64;
	}
	if (player.pos.y < 0)
	{
		player.pos.y = 0;
	}
	if (player.pos.y > 480 - 64)
	{
		player.pos.y = 480 - 64;
	}

	// プレイヤーを描画
	DrawGraph((int)(player.pos.x), (int)(player.pos.y), player.graph, FALSE);
}

// エネミーの初期化
void InitEnemy()
{
	// エネミーのグラフィックをメモリにロード＆表示座標を初期化
	enemy.graph = LoadGraph("data/texture/enemy.png");
	enemy.pos.x = 0;
	enemy.pos.y = 50;

	// 右移動しているかどうかのフラグをtrueに
	enemy.rightMove = true;
}

// エネミーの更新
void UpdateEnemy()
{
	// エネミーの座標を移動している方向に移動する
	VECTOR enemyDirection = VGet(0, 0, 0);
	float enemySpeed = 3.0f;
	if (enemy.rightMove == true)
	{
		enemyDirection = VAdd(enemyDirection, VGet(1, 0, 0));
	}
	else
	{
		enemyDirection = VAdd(enemyDirection, VGet(-1, 0, 0));
	}
	if (VSize(enemyDirection) > 0)
	{
		enemyDirection = VNorm(enemyDirection);
	}
	VECTOR enemyVelocity = VScale(enemyDirection, enemySpeed);
	enemy.pos = VAdd(enemy.pos, enemyVelocity);

	// エネミーが画面端からでそうになっていたら画面内の座標に戻してあげ、移動する方向も反転する
	if (enemy.pos.x > 576)
	{
		enemy.pos.x = 576;
		enemy.rightMove = false;
	}
	else if (enemy.pos.x < 0)
	{
		enemy.pos.x = 0;
		enemy.rightMove = true;
	}

	// エネミーを描画
	DrawGraph((int)enemy.pos.x, (int)enemy.pos.y, enemy.graph, FALSE);
}

// ショットの初期化
void InitShot()
{
	// ショットのグラフィックをメモリにロード.
	shot.graph = LoadGraph("data/texture/shot.png");

	shot.lifeCounter = 0;		// ショットの生きている時間カウンタ
	shot.dir = VGet(0, 0, 0);
	shot.isAlive = false;		// 弾が画面上に存在しているか
}

// ショットの更新
void UpdateShot()
{
	// 自機の弾の移動ルーチン( 存在状態を保持している変数の内容がtrue(存在する)の場合のみ行う )
	if (shot.isAlive == true)
	{
		// フレームごとにカウントが増える
		++shot.lifeCounter;

		// 弾iを１６ドット上に移動させる
		float shotSpeed = 16.0f;

		// うたれてから15フレームの間はshot.dirを敵の方向にする
		if (shot.lifeCounter <= 15)
		{
			shot.dir = VSub(enemy.pos, shot.pos);	// enemy.pos - shot.pos
		}

		if (VSize(shot.dir) > 0)
		{
			shot.dir = VNorm(shot.dir);
		}
		VECTOR shotVelocity = VScale(shot.dir, shotSpeed);
		shot.pos = VAdd(shot.pos, shotVelocity);

		// 画面外に出てしまった場合は存在状態を保持している変数にfalse(存在しない)を代入する
		if (shot.pos.y < -80)
		{
			shot.isAlive = false;
		}

		// 画面に弾iを描画する
		DrawGraph((int)shot.pos.x, (int)shot.pos.y, shot.graph, FALSE);
	}
}

//-----------------------------------------------------------------------------
// @brief  メイン関数.
//-----------------------------------------------------------------------------
// WinMain関数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	// 画面モードの設定
	SetGraphMode(640, 480, 16);		// 解像度を640*480、colorを16bitに設定.
	ChangeWindowMode(TRUE);			// ウインドウモードに.

	// ＤＸライブラリ初期化処理
	if (DxLib_Init() == -1)
	{
		return -1;
	}

	// グラフィックの描画先を裏画面にセット
	SetDrawScreen(DX_SCREEN_BACK);
	
	InitPlayer();	// プレイヤーの初期化
	InitEnemy();	// エネミーの初期化
	InitShot();		// ショットの初期化

	// ゲームループ.
	while (1)
	{
		// 画面を初期化(真っ黒にする)
		ClearDrawScreen();

		UpdatePlayer();	// プレイヤーの更新
		UpdateEnemy();	// エネミーの更新
		UpdateShot();	// 弾の更新

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