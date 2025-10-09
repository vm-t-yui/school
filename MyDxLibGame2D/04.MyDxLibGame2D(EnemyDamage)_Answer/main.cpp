//-----------------------------------------------------------------------------
// @brief  メイン処理.
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#include "DxLib.h"
#include <vector>

// 定数群
constexpr int	ScreenW			= 640;
constexpr int	ScreenH			= 480;
constexpr float	PlayerSpeed		= 3.0f;
constexpr float	EnemySpeed		= 3.0f;
constexpr float	EnemyHitSize	= 30;	// エネミーの当たり判定サイズ
constexpr int	EnemyDamageTime	= 30;	// エネミーのダメージ顔になっている時間
const VECTOR	PlayerFirstPos	= VGet(ScreenW * 0.5f, ScreenH - 80.0f, 0);
const VECTOR	EnemyFirstPos	= VGet(0, 50, 0);
constexpr int	ColorBit		= 16;
constexpr int	ShotNum			= 3;
constexpr float	ShotSpeed		= 3.0f;
constexpr float	ShotAliveLimitY = -80.0f;
constexpr float	ShotHitSize		= 10;	// ショットの当たり判定サイズ

// デバッグ表示のカラー
#if _DEBUG
	constexpr unsigned int	DebugEnemyHitSizeColor	= 0xff0000;
	constexpr unsigned int	DebugShotHitSizeColor	= 0x00ffff;
#endif

/// <summary>
/// メイン関数
/// </summary>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	// 画面モードの設定
	SetGraphMode(ScreenW, ScreenH, ColorBit);		// 解像度を640*480、colorを16bitに設定
	ChangeWindowMode(TRUE);							// ウインドウモードに

	// ＤＸライブラリ初期化処理
	if (DxLib_Init() == -1)
	{
		return -1;
	}

	// プレイヤーのグラフィックをメモリにロード＆表示座標を初期化
	VECTOR	playerPos	= PlayerFirstPos;
	VECTOR	playerDir	= VGet(0, 0, 0);	// プレイヤーの向き
	int		playerGraph	= LoadGraph("data/texture/player.png");

	// キャラの画像の大きさを取得。毎度キャストするのがいやなので半分サイズも準備
	int playerW, playerH;
	GetGraphSize(playerGraph, &playerW, &playerH);

	const float playerHalfW = playerW * 0.5f;
	const float playerHalfH = playerH * 0.5f;

	// エネミーのグラフィックをメモリにロード＆表示座標を初期化
	VECTOR	enemyPos	= EnemyFirstPos;
	VECTOR	enemyDir	= VGet(0, 0, 0);	// エネミーの向き
	int		enemyGraphNormal	= LoadGraph("data/texture/enemy.png");
	int		enemyGraphDamage	= LoadGraph("data/texture/enemyDamage.png");
	int		enemyGraph			= enemyGraphNormal;

	int enemyW, enemyH;
	GetGraphSize(enemyGraph, &enemyW, &enemyH);

	const float enemyHalfW = enemyW * 0.5f;
	const float enemyHalfH = enemyH * 0.5f;

	// エネミーが右移動しているかどうかのフラグをリセット
	bool isEnemyRightMove = true;

	// 敵の状態を表す列挙体
	enum class EnemyState : int
	{
		Normal,		// 通常
		Damage		// ダメージ
	};
	EnemyState	enemyState		 = EnemyState::Normal;		// 通常状態で初期化
	int			enemyDamageCount = 0;						// ダメージカウントを初期化

	// --- 全部の弾で共通のデータ
	// ショットのグラフィックをメモリにロード+サイズ取得
	int shotGraph;
	shotGraph = LoadGraph("data/texture/shot.png");

	int shotW, shotH;
	GetGraphSize(shotGraph, &shotW, &shotH);

	const float shotHalfW = shotW * 0.5f;
	const float shotHalfH = shotH * 0.5f;

	bool isPressingShotButton	= false;	// そのフレームでボタンが押されているかどうか保存する
	bool isPressedShotButton	= false;	// ボタンが押された瞬間を保存するフラグ
	bool isPrevInputShotButton	= false;	// 前のフレームにショットボタンのインプットがあったかどうか

	// --- 弾の数だけ存在するデータ
	// 弾の位置、ディレクションを作成
	std::vector<VECTOR> shotPos(ShotNum, VGet(0, 0, 0));
	std::vector<VECTOR> shotDir(ShotNum, VGet(0, -1, 0));	// 弾は常に上にしか移動しない

	// 弾が画面上に存在しているか保持する変数に『存在していない』を意味するfalseを代入しておく
	std::vector<bool> isShotAlive(ShotNum, false);

	// ゲームループ.
	while (1)
	{
		auto prevTime = GetNowHiPerformanceCount();	// 処理が始まる前の時間

		// 画面を初期化(真っ黒にする)
		ClearDrawScreen();

		//------------------------------//
		// プレイヤーの操作ルーチン
		//------------------------------//
		{
			playerDir = VGet(0, 0, 0);	// 向きをリセット

			// 矢印キーを押していたらプレイヤーを移動させる
			if (CheckHitKey(KEY_INPUT_UP) == 1)
			{
				playerDir = VAdd(playerDir, VGet(0, -1, 0));
			}
			if (CheckHitKey(KEY_INPUT_DOWN) == 1)
			{
				playerDir = VAdd(playerDir, VGet(0, 1, 0));
			}
			if (CheckHitKey(KEY_INPUT_LEFT) == 1)
			{
				playerDir = VAdd(playerDir, VGet(-1, 0, 0));
			}
			if (CheckHitKey(KEY_INPUT_RIGHT) == 1)
			{
				playerDir = VAdd(playerDir, VGet(1, 0, 0));
			}

			// 長さがゼロじゃない場合、向きを正規化して、長さ1に
			if (VSize(playerDir) > 0)
			{
				playerDir = VNorm(playerDir);
			}

			// プレイヤーの移動
			VECTOR playerVelocity = VScale(playerDir, PlayerSpeed);	// 長さ1の向きに、大きさ（速度）をかける
			playerPos = VAdd(playerPos, playerVelocity);			// 座標ベクトルに、velicityを足すことで移動

			// 弾の発射処理
			// ボタンが押されているかどうかを保存する
			isPrevInputShotButton = isPressingShotButton;
			if (CheckHitKey(KEY_INPUT_SPACE))
			{
				isPressingShotButton = true;
			}
			else
			{
				isPressingShotButton = false;
			}
			// ボタンが押された瞬間を取得する
			if (isPressingShotButton && !isPrevInputShotButton)
			{
				isPressedShotButton = true;
			}
			else
			{
				isPressedShotButton = false;
			}

			// ボタンが押された瞬間だけ、発射処理を行う
			if (isPressedShotButton)
			{
				// 弾i個分繰り返す
 				for (int i = 0; i < ShotNum; i++)
				{
					// 弾が画面上にでていない場合はその弾を画面に出す
 					if (isShotAlive[i] == false)
					{
						// 弾の発射位置をセット、プレイヤーの中心にする
						shotPos[i] = playerPos;

						// 弾が撃たれたので、存在状態を保持する変数にtrueを代入する
						isShotAlive[i] = true;

						break;	// 一発撃ったら抜ける
					}
				}
			}

			// プレイヤーが画面左端からはみ出そうになっていたら画面内の座標に戻してあげる
			if (playerPos.x < playerHalfW)
			{
				playerPos.x = playerHalfW;
			}
			if (playerPos.x > ScreenW - playerHalfW)
			{
				playerPos.x = ScreenW - playerHalfW;
			}
			if (playerPos.y < playerHalfH)
			{
				playerPos.y = playerHalfH;
			}
			if (playerPos.y > ScreenH - playerHalfH)
			{
				playerPos.y = ScreenH - playerHalfH;
			}

			// プレイヤーを描画
			DrawRotaGraph3(static_cast<int>(playerPos.x),
				static_cast<int>(playerPos.y),
				static_cast<int>(playerHalfW), static_cast<int>(playerHalfH),
				1.0f, 1.0f,
				0.0f, playerGraph,
				FALSE, FALSE);
		}

		//------------------------------//
		// エネミーの移動ルーチン
		//------------------------------//
		{
			// エネミーの移動方向の確定。固定で必ず左右どちらかになる
			if (isEnemyRightMove == true)
			{
				enemyDir = VGet(1, 0, 0);
			}
			else
			{
				enemyDir = VGet(-1, 0, 0);
			}

			// エネミーの状態別処理
			switch (enemyState)
			{
			case EnemyState::Normal:	// 通常なら通常顔に
				enemyGraph = enemyGraphNormal;
				break;
			case EnemyState::Damage:	// ダメージならダメージ顔に。ダメージカウントを小さくする
				enemyGraph = enemyGraphNormal;
				--enemyDamageCount;		// カウントを減らし、ゼロ以下になったら通常状態に戻す
				if (enemyDamageCount <= 0)
				{
					enemyDamageCount = 0;
					enemyState		 = EnemyState::Normal;
				}
				break;
			default:	// それ以外なら何もしない
				break;
			}

			// エネミーの移動。すでに長さ１なので正規化はいらない
			VECTOR enemyVelocity = VScale(enemyDir, EnemySpeed);	// 長さ1の向きに、大きさ（速度）をかける
			enemyPos = VAdd(enemyPos, enemyVelocity);				// 座標ベクトルに、velicityを足すことで移動

			// エネミーが画面端からでそうになっていたら画面内の座標に戻してあげ、移動する方向も反転する
			if (enemyPos.x > ScreenW - enemyHalfW)
			{
				enemyPos.x = ScreenW - enemyHalfW;
				isEnemyRightMove = false;
			}
			else if (enemyPos.x < enemyHalfW)
			{
				enemyPos.x = enemyHalfW;
				isEnemyRightMove = true;
			}

			// エネミーを描画
			DrawRotaGraph3(static_cast<int>(enemyPos.x),
				static_cast<int>(enemyPos.y),
				static_cast<int>(enemyHalfW), static_cast<int>(enemyHalfH),
				1.0f, 1.0f,
				0.0f, enemyGraph,
				FALSE, FALSE);

#if _DEBUG
			// デバッグ表示:敵の当たり判定の描画
			DrawCircle(static_cast<int>(enemyPos.x),
				static_cast<int>(enemyPos.y),
				static_cast<int>(EnemyHitSize),
				DebugEnemyHitSizeColor, 0);
#endif
		}

		//------------------------------//
		// 弾の移動ルーチン
		//------------------------------//
		// 弾i個分繰り返す
		for (int i = 0; i < ShotNum; i++)
		{
			// 存在状態を保持している変数の内容がtrue(存在する)の場合のみ行う
			if (isShotAlive[i] == true)
			{
				// 弾を移動させる。shotDirは常に上方向で長さ１なので、正規化はいらない
				VECTOR shotVelocity = VScale(shotDir[i], ShotSpeed);
				shotPos[i] = VAdd(shotPos[i], shotVelocity);

				// 画面外に出てしまった場合は存在状態を保持している変数にfalse(存在しない)を代入する
				if (shotPos[i].y < ShotAliveLimitY)
				{
					isShotAlive[i] = false;
				}

				// 弾が敵にぶつかっていたら、敵の状態をダメージ状態に
				VECTOR	shotToEnemy			= VSub(enemyPos, shotPos[i]);	// ショットから敵へのベクトル
				float	shotToEnemyLength	= VSize(shotToEnemy);			// ショットから敵への距離
				if (shotToEnemyLength < EnemyHitSize + ShotHitSize)
				{
					// 円（または球）同士の当たり判定
					// ショットから敵への距離がお互いの当たり判定サイズより小さい＝当たっている
					enemyState			= EnemyState::Damage;
					enemyDamageCount	= EnemyDamageTime;
				}

				// 弾を描画する
				DrawRotaGraph3(static_cast<int>(shotPos[i].x),
					static_cast<int>(shotPos[i].y),
					static_cast<int>(shotHalfW), static_cast<int>(shotHalfH),
					1.0f, 1.0f,
					0.0f, shotGraph,
					FALSE, FALSE);

#if _DEBUG
				// デバッグ表示:弾の当たり判定の描画
				DrawCircle(static_cast<int>(shotPos[i].x),
					static_cast<int>(shotPos[i].y),
					static_cast<int>(ShotHitSize),
					DebugShotHitSizeColor, 0);
#endif
			}
		}

		// 雑なfps固定処理
		// 差を求めて、1回の画面更新が1/60秒になるようにwhileループ回して待つ
		auto afterTime = GetNowHiPerformanceCount(); // 処理が終わった後の時間
		while (afterTime - prevTime < 16667)
		{
			afterTime = GetNowHiPerformanceCount();
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