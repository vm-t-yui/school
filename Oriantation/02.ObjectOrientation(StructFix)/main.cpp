//-----------------------------------------------------------------------------
// @brief  メイン処理.
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#include "DxLib.h"

//-----------------------------------------------------------------------------
// @brief  メイン関数.
//-----------------------------------------------------------------------------
// ショットの数.
#define SHOT 20

// Player構造体
struct Player
{
	int x;
	int y;
	int graph;
	bool prevShotFlag;
};

// Enemy構造体
struct Enemy
{
	int x;
	int y;
	int graph;
	int damageFlag;
	int damageCounter;
	int damageGraph;
	int w;
	int h;
	bool rightMove;
};

// Shot構造体
struct Shot
{
	int graph;
	int w;
	int h;
	int x;
	int y;
	bool isVisible;
};


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

	// プレイヤーのグラフィックをメモリにロード＆表示座標を初期化
	Player player;
	player.graph = LoadGraph("data/texture/player.png");
	player.x = 288;
	player.y = 400;

	// エネミーのグラフィックをメモリにロード＆表示座標を初期化
	Enemy enemy;
	enemy.graph = LoadGraph("data/texture/enemy.png");
	enemy.x = 0; 
	enemy.y = 50;

	// エネミーのグラフィックをメモリにロード＆ダメージ時の状態管理用の変数を初期化
	enemy.damageGraph = LoadGraph("data/texture/enemydamage.png");

	// エネミーが顔を歪めているかどうかの変数に『歪めていない』を表す０を代入
	enemy.damageFlag = 0;

	// エネミーが右に移動しているかどうかのフラグにtrueを代入.
	enemy.rightMove = true;

	// ショットのグラフィックをメモリにロード.
	Shot shot[SHOT];
	int localShotGraph = LoadGraph("data/texture/shot.png");
	for (int i = 0; i < SHOT; i++)
	{
		shot[i].graph = localShotGraph;
	}

	// 弾が画面上に存在しているか保持する変数に『存在していない』を意味するfalseを代入しておく
	for (int i = 0; i < SHOT; i++)
	{
		shot[i].isVisible= false;
	}

	// 弾のグラフィックのサイズをえる
	int localShotW;
	int localShotH;
	GetGraphSize(localShotGraph, &localShotW, &localShotH);
	for (int i = 0; i < SHOT; i++)
	{
		shot[i].w = localShotW;
		shot[i].h = localShotH;
	}

	// エネミーのグラフィックのサイズを得る
	GetGraphSize(enemy.graph, &enemy.w, &enemy.h);

	// ショットボタンが前のフレームで押されたかどうかを保存する変数にfalse(押されいない)を代入
	player.prevShotFlag = false;

	// エネミーが右移動しているかどうかのフラグをリセット
	enemy.rightMove = true;

	// ゲームループ.
	while (1)
	{
		// 画面を初期化(真っ黒にする)
		ClearDrawScreen();

		//------------------------------//
		// プレイヤールーチン
		//------------------------------//
		{
			// 矢印キーを押していたらプレイヤーを移動させる
			if (CheckHitKey(KEY_INPUT_UP) == 1)
			{
				player.y -= 3;
			}
			if (CheckHitKey(KEY_INPUT_DOWN) == 1)
			{
				player.y += 3;
			}
			if (CheckHitKey(KEY_INPUT_LEFT) == 1)
			{
				player.x -= 3;
			}
			if (CheckHitKey(KEY_INPUT_RIGHT) == 1)
			{
				player.x += 3;
			}

			// 弾の発射処理
			if (CheckHitKey(KEY_INPUT_SPACE))
			{
				// 前フレームでショットボタンを押したかが保存されている変数がfalseだったら弾を発射
				if (player.prevShotFlag == false)
				{
					// 画面上にでていない弾があるか、弾の数だけ繰り返して調べる
					for (int i = 0; i < SHOT; i++)
					{
						// 弾iが画面上にでていない場合はその弾を画面に出す
						if (shot[i].isVisible== false)
						{
							int Bw, Bh, Sw, Sh;

							// プレイヤーと弾の画像のサイズを得る
							GetGraphSize(player.graph, &Bw, &Bh);
							GetGraphSize(shot[i].graph, &Sw, &Sh);

							// 弾iの位置をセット、位置はプレイヤーの中心にする
							shot[i].x = (Bw - Sw) / 2 + player.x;
							shot[i].y = (Bh - Sh) / 2 + player.y;

							// 弾iは現時点を持って存在するので、存在状態を保持する変数にtrueを代入する
							shot[i].isVisible= true;

							// 一つ弾を出したので弾を出すループから抜けます
							break;
						}
					}
				}

				// 前フレームでショットボタンを押されていたかを保存する変数にtrue(おされていた)を代入
				player.prevShotFlag = true;
			}
			else
			{
				// ショットボタンが押されていなかった場合は
				// 前フレームでショットボタンが押されていたかを保存する変数にfalse(おされていない)を代入
				player.prevShotFlag = false;
			}

			// プレイヤーが画面左端からはみ出そうになっていたら画面内の座標に戻してあげる
			if (player.x < 0)
			{
				player.x = 0;
			}
			if (player.x > 640 - 64)
			{
				player.x = 640 - 64;
			}
			if (player.y < 0)
			{
				player.y = 0;
			}
			if (player.y > 480 - 64)
			{
				player.y = 480 - 64;
			}

			// プレイヤーを描画
			DrawGraph(player.x, player.y, player.graph, FALSE);
		}

		//------------------------------//
		// エネミールーチン
		//------------------------------//
		{
			// エネミーの座標を移動している方向に移動する
			if (enemy.rightMove == true)
			{
				enemy.x += 3;
			}
			else
			{
				enemy.x -= 3;
			}

			// エネミーが画面端からでそうになっていたら画面内の座標に戻してあげ、移動する方向も反転する
			if (enemy.x > 576)
			{
				enemy.x = 576;
				enemy.rightMove = false;
			}
			else if (enemy.x < 0)
			{
				enemy.x = 0;
				enemy.rightMove = true;
			}

			// エネミーを描画
			// 顔を歪めているかどうかで処理を分岐
			if (enemy.damageFlag == 1)
			{
				// 顔を歪めている場合はダメージ時のグラフィックを描画する
				DrawGraph(enemy.x, enemy.y, enemy.damageGraph, FALSE);

				// 顔を歪めている時間を測るカウンターに１を加算する
				enemy.damageCounter++;

				// もし顔を歪め初めて ３０ フレーム経過していたら顔の歪んだ状態から
				// 元に戻してあげる
				if (enemy.damageCounter == 30)
				{
					// 『歪んでいない』を表す０を代入
					enemy.damageFlag = 0;
				}
			}
			else
			{
				DrawGraph(enemy.x, enemy.y, enemy.graph, FALSE);
			}
		}

		//------------------------------//
		// 弾ルーチン
		//------------------------------//
		for (int i = 0; i < SHOT; i++)
		{
			// 自機の弾iの移動ルーチン( 存在状態を保持している変数の内容がtrue(存在する)の場合のみ行う )
			if (shot[i].isVisible== true)
			{
				// 弾iを１６ドット上に移動させる
				shot[i].y -= 16;

				// 画面外に出てしまった場合は存在状態を保持している変数にfalse(存在しない)を代入する
				if (shot[i].y < -80)
				{
					shot[i].isVisible= false;
				}

				// 画面に弾iを描画する
				DrawGraph(shot[i].x, shot[i].y, shot[i].graph, FALSE);
			}

			// 弾のあたり判定.
			// 弾iが存在している場合のみ次の処理に映る
			if (shot[i].isVisible== 1)
			{
				// エネミーとの当たり判定
				if (((shot[i].x > enemy.x && shot[i].x < enemy.x + enemy.w) ||
					(enemy.x > shot[i].x && enemy.x < shot[i].x + shot[i].w)) &&
					((shot[i].y > enemy.y && shot[i].y < enemy.y + enemy.h) ||
						(enemy.y > shot[i].y && enemy.y < shot[i].y + shot[i].h)))
				{
					// 接触している場合は当たった弾の存在を消す
					shot[i].isVisible= 0;

					// エネミーの顔を歪めているかどうかを保持する変数に『歪めている』を表す１を代入
					enemy.damageFlag = 1;

					// エネミーの顔を歪めている時間を測るカウンタ変数に０を代入
					enemy.damageCounter = 0;
				}
			}
		}

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