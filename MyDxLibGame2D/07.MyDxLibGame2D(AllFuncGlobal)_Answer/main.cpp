//-----------------------------------------------------------------------------
// @brief  メイン処理.
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#include "DxLib.h"
#include <vector>

/// <summary>
/// グラフィック関係の定数群
/// </summary>
namespace Graphics
{
	constexpr int	ScreenW			= 640;
	constexpr int	ScreenH			= 480;
	constexpr int	ColorBit		= 16;
	constexpr int	OneFrameNanoSec = 16667;
}

// デバッグ表示のカラー
#if _DEBUG
namespace Debug
{
	constexpr unsigned int	EnemyHitSizeColor = 0xff0000;
	constexpr unsigned int	ShotHitSizeColor = 0x00ffff;
}
#endif

/// <summary>
/// プレイヤークラス
/// </summary>
	class Player
{
public:
	VECTOR	pos;
	VECTOR	dir;
	int		graph;
	int		w, h;

	// constexpr定数は後ろにstatic(C++17以降)
	constexpr static float	Speed	= 3.0f;

	// constは頭にinline,後ろにstatic(C++17以降)
	inline const static	VECTOR	FirstPos	= VGet(Graphics::ScreenW * 0.5f, Graphics::ScreenH - 80.0f, 0);
};

/// <summary>
/// エネミークラス
/// </summary>
class Enemy
{
public:
	// 敵の状態を表す列挙体
	enum class State : int
	{
		Normal,		// 通常
		Damage		// ダメージ
	};

	VECTOR	pos;
	VECTOR	dir;	// エネミーの向き
	int		graphNormal;
	int		graphDamage;
	int		graph;
	int		w;
	int		h;
	bool	isRightMove;
	State	state;
	int		damageCount;
	
	constexpr static float		Speed		= 3.0f;
	constexpr static float		HitSize		= 30;	// エネミーの当たり判定サイズ
	constexpr static int		DamageTime	= 30;	// エネミーのダメージ顔になっている時間
	inline const static VECTOR	FirstPos	= VGet(0, 50, 0);
};

/// <summary>
/// ショットクラス
/// </summary>
class Shot
{
public:
	int graph;
	int w;
	int h;

	VECTOR	pos;
	VECTOR	dir;
	bool	isAlive;

	constexpr static int	Num			= 3;
	constexpr static float	Speed		= 3.0f;
	constexpr static float	AliveLimitY	= -80.0f;
	constexpr static float	HitSize		= 10;		// ショットの当たり判定サイズ
};

/// <summary>
/// キー入力クラス
/// </summary>
class Input
{
public:
	bool isPressingShotButton;	// そのフレームでボタンが押されているかどうか
	bool isPressedShotButton;	// ボタンが押された瞬間を保存するフラグ
	bool isPrevInputShotButton;	// 前のフレームにショットボタンのインプットがあったかどうか

};

// グローバル変数群
Input				input;
Player				player;
Enemy				enemy;
std::vector<Shot>	shot(Shot::Num);

/// <summary>
/// インプットクラス初期化
/// </summary>
void InitializeInput()
{
	input.isPressingShotButton = false;	// そのフレームでボタンが押されているかどうか
	input.isPressedShotButton = false;	// ボタンが押された瞬間を保存するフラグ
	input.isPrevInputShotButton = false;	// 前のフレームにショットボタンのインプットがあったかどうか
}

/// <summary>
/// インプットクラス更新
/// </summary>
void UpdateInput()
{
	// ボタンが押されているかどうかを保存する
	input.isPrevInputShotButton = input.isPressingShotButton;
	if (CheckHitKey(KEY_INPUT_SPACE))
	{
		input.isPressingShotButton = true;
	}
	else
	{
		input.isPressingShotButton = false;
	}
	// ボタンが押された瞬間を取得する
	if (input.isPressingShotButton && !input.isPrevInputShotButton)
	{
		input.isPressedShotButton = true;
	}
	else
	{
		input.isPressedShotButton = false;
	}
}

/// <summary>
/// プレイヤーの初期化
/// </summary>
void InitializePlayer()
{
	// プレイヤーのグラフィックをメモリにロード＆表示座標を初期化
	player.pos = Player::FirstPos;
	player.dir = VGet(0, 0, 0);	// プレイヤーの向き
	player.graph = LoadGraph("data/texture/player.png");

	// キャラの画像の大きさを取得。毎度キャストするのがいやなので半分サイズも準備
	GetGraphSize(player.graph, &player.w, &player.h);
}

/// <summary>
/// プレイヤーの更新
/// </summary>
void UpdatePlayer()
{
	player.dir = VGet(0, 0, 0);	// 向きをリセット

	// 矢印キーを押していたらプレイヤーを移動させる
	if (CheckHitKey(KEY_INPUT_UP) == 1)
	{
		player.dir = VAdd(player.dir, VGet(0, -1, 0));
	}
	if (CheckHitKey(KEY_INPUT_DOWN) == 1)
	{
		player.dir = VAdd(player.dir, VGet(0, 1, 0));
	}
	if (CheckHitKey(KEY_INPUT_LEFT) == 1)
	{
		player.dir = VAdd(player.dir, VGet(-1, 0, 0));
	}
	if (CheckHitKey(KEY_INPUT_RIGHT) == 1)
	{
		player.dir = VAdd(player.dir, VGet(1, 0, 0));
	}

	// 長さがゼロじゃない場合、向きを正規化して、長さ1に
	if (VSize(player.dir) > 0)
	{
		player.dir = VNorm(player.dir);
	}

	// プレイヤーの移動
	VECTOR playerVelocity = VScale(player.dir, Player::Speed);	// 長さ1の向きに、大きさ（速度）をかける
	player.pos = VAdd(player.pos, playerVelocity);				// 座標ベクトルに、velicityを足すことで移動

	// 弾の発射処理
	// ボタンが押された瞬間だけ、発射処理を行う
	if (input.isPressedShotButton)
	{
		// 弾i個分繰り返す
		for (int i = 0; i < Shot::Num; i++)
		{
			// 弾が画面上にでていない場合はその弾を画面に出す
			if (shot[i].isAlive == false)
			{
				// 弾の発射位置をセット、プレイヤーの中心にする
				shot[i].pos = player.pos;

				// 弾が撃たれたので、存在状態を保持する変数にtrueを代入する
				shot[i].isAlive = true;

				break;	// 一発撃ったら抜ける
			}
		}
	}

	const float playerHalfW = player.w * 0.5f;
	const float playerHalfH = player.h * 0.5f;

	// プレイヤーが画面左端からはみ出そうになっていたら画面内の座標に戻してあげる
	if (player.pos.x < playerHalfW)
	{
		player.pos.x = playerHalfW;
	}
	if (player.pos.x > Graphics::ScreenW - playerHalfW)
	{
		player.pos.x = Graphics::ScreenW - playerHalfW;
	}
	if (player.pos.y < playerHalfH)
	{
		player.pos.y = playerHalfH;
	}
	if (player.pos.y > Graphics::ScreenH - playerHalfH)
	{
		player.pos.y = Graphics::ScreenH - playerHalfH;
	}
}

/// <summary>
/// プレイヤーを描画
/// </summary>
void DrawPlayer()
{
	const float playerHalfW = player.w * 0.5f;
	const float playerHalfH = player.h * 0.5f;

	DrawRotaGraph3(static_cast<int>(player.pos.x),
		static_cast<int>(player.pos.y),
		static_cast<int>(playerHalfW), static_cast<int>(playerHalfH),
		1.0f, 1.0f,
		0.0f, player.graph,
		FALSE, FALSE);
}

/// <summary>
/// 敵の初期化
/// </summary>
void InitializeEnemy()
{
	// エネミーのグラフィックをメモリにロード＆表示座標を初期化
	enemy.pos = Enemy::FirstPos;
	enemy.dir = VGet(0, 0, 0);	// エネミーの向き
	enemy.graphNormal = LoadGraph("data/texture/enemy.png");
	enemy.graphDamage = LoadGraph("data/texture/enemyDamage.png");
	enemy.graph = enemy.graphNormal;

	GetGraphSize(enemy.graph, &enemy.w, &enemy.h);

	// エネミーが右移動しているかどうかのフラグをリセット
	enemy.isRightMove = true;
	enemy.state = Enemy::State::Normal;		// 通常状態で初期化
	enemy.damageCount = 0;						// ダメージカウントを初期化
}

/// <summary>
/// エネミーの更新
/// </summary>
void UpdateEnemy()
{
	// エネミーの移動方向の確定。固定で必ず左右どちらかになる
	if (enemy.isRightMove == true)
	{
		enemy.dir = VGet(1, 0, 0);
	}
	else
	{
		enemy.dir = VGet(-1, 0, 0);
	}

	// エネミーの状態別処理
	switch (enemy.state)
	{
	case Enemy::State::Normal:	// 通常なら通常顔に
		enemy.graph = enemy.graphNormal;
		break;
	case Enemy::State::Damage:	// ダメージならダメージ顔に。ダメージカウントを小さくする
		enemy.graph = enemy.graphDamage;
		--enemy.damageCount;		// カウントを減らし、ゼロ以下になったら通常状態に戻す
		if (enemy.damageCount <= 0)
		{
			enemy.damageCount = 0;
			enemy.state = Enemy::State::Normal;
		}
		break;
	default:	// それ以外なら何もしない
		break;
	}

	const float enemyHalfW = enemy.w * 0.5f;
	const float enemyHalfH = enemy.h * 0.5f;

	// エネミーの移動。すでに長さ１なので正規化はいらない
	VECTOR enemyVelocity = VScale(enemy.dir, Enemy::Speed);	// 長さ1の向きに、大きさ（速度）をかける
	enemy.pos = VAdd(enemy.pos, enemyVelocity);				// 座標ベクトルに、velicityを足すことで移動

	// エネミーが画面端からでそうになっていたら画面内の座標に戻してあげ、移動する方向も反転する
	if (enemy.pos.x > Graphics::ScreenW - enemyHalfW)
	{
		enemy.pos.x = Graphics::ScreenW - enemyHalfW;
		enemy.isRightMove = false;
	}
	else if (enemy.pos.x < enemyHalfW)
	{
		enemy.pos.x = enemyHalfW;
		enemy.isRightMove = true;
	}
}

/// <summary>
/// エネミー描画
/// </summary>
void DrawEnemy()
{
	const float enemyHalfW = enemy.w * 0.5f;
	const float enemyHalfH = enemy.h * 0.5f;

	// エネミーを描画
	DrawRotaGraph3(static_cast<int>(enemy.pos.x),
		static_cast<int>(enemy.pos.y),
		static_cast<int>(enemyHalfW), static_cast<int>(enemyHalfH),
		1.0f, 1.0f,
		0.0f, enemy.graph,
		FALSE, FALSE);

#if _DEBUG
	// デバッグ表示:敵の当たり判定の描画
	DrawCircle(static_cast<int>(enemy.pos.x),
		static_cast<int>(enemy.pos.y),
		static_cast<int>(Enemy::HitSize),
		Debug::EnemyHitSizeColor, 0);
#endif
}

/// <summary>
/// ショット初期化
/// </summary>
void InitializeShot()
{
	// --- 全部の弾で共通のデータ
	// ショットのグラフィックをメモリにロード+サイズ取得
	for (int i = 0; i < Shot::Num; i++)
	{
		shot[i].graph = LoadGraph("data/texture/shot.png");

		GetGraphSize(shot[i].graph, &shot[i].w, &shot[i].h);
	}

	// --- 弾の数だけ存在するデータ
	// 弾の位置、ディレクションを作成
	for (int i = 0; i < Shot::Num; i++)
	{
		shot[i].pos = VGet(0, 0, 0);
		shot[i].dir = VGet(0, -1, 0);	// 弾は常に上にしか移動しない
	}

	// 弾が画面上に存在しているか保持する変数に『存在していない』を意味するfalseを代入しておく
	for (int i = 0; i < Shot::Num; i++)
	{
		shot[i].isAlive = false;
	}
}

/// <summary>
/// ショットの更新
/// </summary>
void UpdateShot()
{
	// 弾i個分繰り返す
	for (int i = 0; i < Shot::Num; i++)
	{
		// 存在状態を保持している変数の内容がtrue(存在する)の場合のみ行う
		if (shot[i].isAlive == true)
		{
			// 弾を移動させる。shot[i].dirは常に上方向で長さ１なので、正規化はいらない
			VECTOR shotVelocity = VScale(shot[i].dir, Shot::Speed);
			shot[i].pos = VAdd(shot[i].pos, shotVelocity);

			// 画面外に出てしまった場合は存在状態を保持している変数にfalse(存在しない)を代入する
			if (shot[i].pos.y < Shot::AliveLimitY)
			{
				shot[i].isAlive = false;
			}

			// 弾が敵にぶつかっていたら、敵の状態をダメージ状態に
			VECTOR	shotToEnemy = VSub(enemy.pos, shot[i].pos);	// ショットから敵へのベクトル
			float	shotToEnemyLength = VSize(shotToEnemy);			// ショットから敵への距離
			if (shotToEnemyLength < Enemy::HitSize + Shot::HitSize)
			{
				// 円（または球）同士の当たり判定
				// ショットから敵への距離がお互いの当たり判定サイズより小さい＝当たっている
				enemy.state = Enemy::State::Damage;
				enemy.damageCount = Enemy::DamageTime;

				// 弾も消す
				shot[i].isAlive = false;
			}
		}
	}
}

/// <summary>
///  ショット描画
/// </summary>
void DrawShot()
{
	///////////////////////////////////////////////////////////////////////////////
	//HACK: 処理を関数化するために移動したとき、for文とif文の再構築を忘れない！！
	///////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < Shot::Num; i++)
	{
		if (shot[i].isAlive == true)
		{
			// 弾を描画する
			const float shotHalfW = shot[i].w * 0.5f;
			const float shotHalfH = shot[i].h * 0.5f;
			DrawRotaGraph3(static_cast<int>(shot[i].pos.x),
				static_cast<int>(shot[i].pos.y),
				static_cast<int>(shotHalfW), static_cast<int>(shotHalfH),
				1.0f, 1.0f,
				0.0f, shot[i].graph,
				FALSE, FALSE);

#if _DEBUG
			// デバッグ表示:弾の当たり判定の描画
			DrawCircle(static_cast<int>(shot[i].pos.x),
				static_cast<int>(shot[i].pos.y),
				static_cast<int>(Shot::HitSize),
				Debug::ShotHitSizeColor, 0);
#endif
		}
	}
}

/// <summary>
/// メイン関数
/// </summary>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	// 画面モードの設定
	SetGraphMode(Graphics::ScreenW, Graphics::ScreenH, Graphics::ColorBit);	// 解像度を640*480、colorを16bitに設定
	ChangeWindowMode(TRUE);							// ウインドウモードに

	// ＤＸライブラリ初期化処理
	if (DxLib_Init() == -1)
	{
		return -1;
	}

	// 各クラスの初期化
	InitializeInput();
	InitializePlayer();
	InitializeEnemy();
	InitializeShot();

	// ゲームループ.
	while (1)
	{
		auto prevTime = GetNowHiPerformanceCount();	// 処理が始まる前の時間

		// 画面を初期化(真っ黒にする)
		ClearDrawScreen();
		
		// 各クラスのUpdate
		UpdateInput();		// ボタン情報を更新
		UpdatePlayer();
		UpdateEnemy();
		UpdateShot();

		// 各クラスのDraw
		DrawPlayer();
		DrawEnemy();
		DrawShot();

		// 裏画面の内容を表画面にコピーする（描画の確定）
		ScreenFlip();

		// 雑なfps固定処理
		// 差を求めて、1回の画面更新が1/60秒になるようにwhileループ回して待つ
		auto afterTime = GetNowHiPerformanceCount(); // 処理が終わった後の時間
		while (afterTime - prevTime < Graphics::OneFrameNanoSec)
		{
			afterTime = GetNowHiPerformanceCount();
		}

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