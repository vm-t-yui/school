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

// プレイヤー構造体.
struct Player
{
	int Graph;
	int X;
	int Y;
};

// エネミー構造体.
struct Enemy
{
	int		X;
	int		Y; 
	int		Graph;
	int		DamageFlag;
	int		DamageCounter;
	int		DamageGraph;
	int		W;
	int		H;

	// エネミーが右移動しているかどうかのフラグ
	bool RightMove = true;
};

// ショット構造体.
struct Shot
{
	int		Graph;
	int		X;
	int		Y;
	bool	Flag;
	int		W;
	int		H;
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
	player.Graph = LoadGraph("data/texture/player.png");
	player.X = 288; 
	player.Y = 400;

	// エネミーのグラフィックをメモリにロード＆表示座標を初期化
	Enemy enemy;
	enemy.Graph = LoadGraph("data/texture/enemy.png");
	enemy.X = 0; 
	enemy.Y = 50;

	// エネミーのグラフィックをメモリにロード＆ダメージ時の状態管理用の変数を初期化
	enemy.DamageGraph = LoadGraph("data/texture/enemyDamage.png");

	// エネミーが顔を歪めているかどうかの変数に『歪めていない』を表す０を代入
	enemy.DamageFlag = 0;

	// ショットのグラフィックをメモリにロード.
	Shot shot[SHOT];
	for (int i = 0; i < SHOT; i++)
	{
		shot[i].Graph = LoadGraph("data/texture/shot.png");
	}

	// 弾が画面上に存在しているか保持する変数に『存在していない』を意味するfalseを代入しておく
	for (int i = 0; i < SHOT; i++)
	{
		shot[i].Flag = false;
	}

	// 弾のグラフィックのサイズをえる
	for (int i = 0; i < SHOT; i++)
	{
		GetGraphSize(shot[i].Graph, &shot[i].W, &shot[i].H);
	}

	// エネミーのグラフィックのサイズを得る
	GetGraphSize(enemy.Graph, &enemy.W, &enemy.H);

	// ショットボタンが前のフレームで押されたかどうかを保存する変数にfalse(押されいない)を代入
	bool prevShotFlag = false;

	// エネミーが右移動しているかどうかのフラグをリセット
	enemy.RightMove = true;

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
				player.Y -= 3;
			}
			if (CheckHitKey(KEY_INPUT_DOWN) == 1)
			{
				player.Y += 3;
			}
			if (CheckHitKey(KEY_INPUT_LEFT) == 1)
			{
				player.X -= 3;
			}
			if (CheckHitKey(KEY_INPUT_RIGHT) == 1)
			{
				player.X += 3;
			}

			// 弾の発射処理
			if (CheckHitKey(KEY_INPUT_SPACE))
			{
				// 前フレームでショットボタンを押したかが保存されている変数がfalseだったら弾を発射
				if (prevShotFlag == false)
				{
					// 画面上にでていない弾があるか、弾の数だけ繰り返して調べる
					for (int i = 0; i < SHOT; i++)
					{
						// 弾iが画面上にでていない場合はその弾を画面に出す
						if (shot[i].Flag == false)
						{
							int Bw, Bh, Sw, Sh;

							// プレイヤーと弾の画像のサイズを得る
							GetGraphSize(player.Graph, &Bw, &Bh);
							GetGraphSize(shot[i].Graph, &Sw, &Sh);

							// 弾iの位置をセット、位置はプレイヤーの中心にする
							shot[i].X = (Bw - Sw) / 2 + player.X;
							shot[i].Y = (Bh - Sh) / 2 + player.Y;

							// 弾iは現時点を持って存在するので、存在状態を保持する変数にtrueを代入する
							shot[i].Flag = true;

							// 一つ弾を出したので弾を出すループから抜けます
							break;
						}
					}
				}

				// 前フレームでショットボタンを押されていたかを保存する変数にtrue(おされていた)を代入
				prevShotFlag = true;
			}
			else
			{
				// ショットボタンが押されていなかった場合は
				// 前フレームでショットボタンが押されていたかを保存する変数にfalse(おされていない)を代入
				prevShotFlag = false;
			}

			// プレイヤーが画面左端からはみ出そうになっていたら画面内の座標に戻してあげる
			if (player.X < 0)
			{
				player.X = 0;
			}
			if (player.X > 640 - 64)
			{
				player.X = 640 - 64;
			}
			if (player.Y < 0)
			{
				player.Y = 0;
			}
			if (player.Y > 480 - 64)
			{
				player.Y = 480 - 64;
			}

			// プレイヤーを描画
			DrawGraph(player.X, player.Y, player.Graph, FALSE);
		}

		//------------------------------//
		// エネミールーチン
		//------------------------------//
		{
			// エネミーの座標を移動している方向に移動する
			if (enemy.RightMove == true)
			{
				enemy.X += 3;
			}
			else
			{
				enemy.X -= 3;
			}

			// エネミーが画面端からでそうになっていたら画面内の座標に戻してあげ、移動する方向も反転する
			if (enemy.X > 576)
			{
				enemy.X = 576;
				enemy.RightMove = false;
			}
			else if (enemy.X < 0)
			{
				enemy.X = 0;
				enemy.RightMove = true;
			}

			// エネミーを描画
			// 顔を歪めているかどうかで処理を分岐
			if (enemy.DamageFlag == 1)
			{
				// 顔を歪めている場合はダメージ時のグラフィックを描画する
				DrawGraph(enemy.X, enemy.Y, enemy.DamageGraph, FALSE);

				// 顔を歪めている時間を測るカウンターに１を加算する
				enemy.DamageCounter++;

				// もし顔を歪め初めて ３０ フレーム経過していたら顔の歪んだ状態から
				// 元に戻してあげる
				if (enemy.DamageCounter == 30)
				{
					// 『歪んでいない』を表す０を代入
					enemy.DamageFlag = 0;
				}
			}
			else
			{
				DrawGraph(enemy.X, enemy.Y, enemy.Graph, FALSE);
			}
		}

		//------------------------------//
		// 弾ルーチン
		//------------------------------//
		for (int i = 0; i < SHOT; i++)
		{
			// 自機の弾iの移動ルーチン( 存在状態を保持している変数の内容がtrue(存在する)の場合のみ行う )
			if (shot[i].Flag == true)
			{
				// 弾iを１６ドット上に移動させる
				shot[i].Y -= 16;

				// 画面外に出てしまった場合は存在状態を保持している変数にfalse(存在しない)を代入する
				if (shot[i].Y < -80)
				{
					shot[i].Flag = false;
				}

				// 画面に弾iを描画する
				DrawGraph(shot[i].X, shot[i].Y, shot[i].Graph, FALSE);
			}

			// 弾のあたり判定.
			// 弾iが存在している場合のみ次の処理に映る
			if (shot[i].Flag == 1)
			{
				// エネミーとの当たり判定
				if (((shot[i].X > enemy.X && shot[i].X < enemy.X + enemy.W) ||
					(enemy.X > shot[i].X && enemy.X < shot[i].X + shot[i].W)) &&
					((shot[i].Y > enemy.Y && shot[i].Y < enemy.Y + enemy.H) ||
						(enemy.Y > shot[i].Y && enemy.Y < shot[i].Y + shot[i].H)))
				{
					// 接触している場合は当たった弾の存在を消す
					shot[i].Flag = 0;

					// エネミーの顔を歪めているかどうかを保持する変数に『歪めている』を表す１を代入
					enemy.DamageFlag = 1;

					// エネミーの顔を歪めている時間を測るカウンタ変数に０を代入
					enemy.DamageCounter = 0;
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