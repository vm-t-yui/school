//-----------------------------------------------------------------------------
// @brief  メイン処理.
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#include "DxLib.h"
#include <vector>
#include "GlobalConstants.h"
#include "Input.h"

/// <summary>
/// プレイヤークラス
/// </summary>
class Input;	// プロトタイプ宣言
class Shot;		// プロトタイプ宣言
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

	void Initialize();
	void Update(const Input& input, std::vector<Shot>& shotArray);
	void Draw() const;
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
	
	void Initialize();
	void Update();
	void Draw() const;
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

	constexpr static int	Num = 3;
	constexpr static float	Speed = 3.0f;
	constexpr static float	AliveLimitY = -80.0f;
	constexpr static float	HitSize = 10;		// ショットの当たり判定サイズ

	void Initialize();
	void Update(Enemy& enemy);
	void Draw() const;
};

/// <summary>
/// プレイヤーの初期化
/// </summary>
void Player::Initialize()
{
	// プレイヤーのグラフィックをメモリにロード＆表示座標を初期化
	pos = Player::FirstPos;
	dir = VGet(0, 0, 0);	// プレイヤーの向き
	graph = LoadGraph("data/texture/player.png");

	// キャラの画像の大きさを取得。毎度キャストするのがいやなので半分サイズも準備
	GetGraphSize(graph, &w, &h);
}

/// <summary>
/// プレイヤーの更新
/// </summary>
void Player::Update(const Input& input, std::vector<Shot>& shotArray)
{
	dir = VGet(0, 0, 0);	// 向きをリセット

	// 矢印キーを押していたらプレイヤーを移動させる
	if (CheckHitKey(KEY_INPUT_UP) == 1)
	{
		dir = VAdd(dir, VGet(0, -1, 0));
	}
	if (CheckHitKey(KEY_INPUT_DOWN) == 1)
	{
		dir = VAdd(dir, VGet(0, 1, 0));
	}
	if (CheckHitKey(KEY_INPUT_LEFT) == 1)
	{
		dir = VAdd(dir, VGet(-1, 0, 0));
	}
	if (CheckHitKey(KEY_INPUT_RIGHT) == 1)
	{
		dir = VAdd(dir, VGet(1, 0, 0));
	}

	// 長さがゼロじゃない場合、向きを正規化して、長さ1に
	if (VSize(dir) > 0)
	{
		dir = VNorm(dir);
	}

	// プレイヤーの移動
	VECTOR playerVelocity = VScale(dir, Player::Speed);	// 長さ1の向きに、大きさ（速度）をかける
	pos = VAdd(pos, playerVelocity);				// 座標ベクトルに、velicityを足すことで移動

	// 弾の発射処理
	// ボタンが押された瞬間だけ、発射処理を行う
	if (input.isPressedShotButton)
	{
		// 弾i個分繰り返す
		for (int i = 0; i < Shot::Num; i++)
		{
			// 弾が画面上にでていない場合はその弾を画面に出す
			if (shotArray[i].isAlive == false)
			{
				// 弾の発射位置をセット、プレイヤーの中心にする
				shotArray[i].pos = pos;

				// 弾が撃たれたので、存在状態を保持する変数にtrueを代入する
				shotArray[i].isAlive = true;

				break;	// 一発撃ったら抜ける
			}
		}
	}

	const float playerHalfW = w * 0.5f;
	const float playerHalfH = h * 0.5f;

	// プレイヤーが画面左端からはみ出そうになっていたら画面内の座標に戻してあげる
	if (pos.x < playerHalfW)
	{
		pos.x = playerHalfW;
	}
	if (pos.x > Graphics::ScreenW - playerHalfW)
	{
		pos.x = Graphics::ScreenW - playerHalfW;
	}
	if (pos.y < playerHalfH)
	{
		pos.y = playerHalfH;
	}
	if (pos.y > Graphics::ScreenH - playerHalfH)
	{
		pos.y = Graphics::ScreenH - playerHalfH;
	}
}

/// <summary>
/// プレイヤーを描画
/// </summary>
void Player::Draw() const
{
	const float playerHalfW = w * 0.5f;
	const float playerHalfH = h * 0.5f;

	DrawRotaGraph3(static_cast<int>(pos.x),
		static_cast<int>(pos.y),
		static_cast<int>(playerHalfW), static_cast<int>(playerHalfH),
		1.0f, 1.0f,
		0.0f, graph,
		FALSE, FALSE);
}

/// <summary>
/// 敵の初期化
/// </summary>
void Enemy::Initialize()
{
	// エネミーのグラフィックをメモリにロード＆表示座標を初期化
	pos			= Enemy::FirstPos;
	dir			= VGet(0, 0, 0);	// エネミーの向き
	graphNormal	= LoadGraph("data/texture/enemy.png");
	graphDamage	= LoadGraph("data/texture/enemyDamage.png");
	graph		= graphNormal;

	GetGraphSize(graph, &w, &h);

	// エネミーが右移動しているかどうかのフラグをリセット
	isRightMove	= true;
	state		= Enemy::State::Normal;		// 通常状態で初期化
	damageCount	= 0;						// ダメージカウントを初期化
}

/// <summary>
/// エネミーの更新
/// </summary>
void Enemy::Update()
{
	// エネミーの移動方向の確定。固定で必ず左右どちらかになる
	if (isRightMove == true)
	{
		dir = VGet(1, 0, 0);
	}
	else
	{
		dir = VGet(-1, 0, 0);
	}

	// エネミーの状態別処理
	switch (state)
	{
	case Enemy::State::Normal:	// 通常なら通常顔に
		graph = graphNormal;
		break;
	case Enemy::State::Damage:	// ダメージならダメージ顔に。ダメージカウントを小さくする
		graph = graphDamage;
		--damageCount;		// カウントを減らし、ゼロ以下になったら通常状態に戻す
		if (damageCount <= 0)
		{
			damageCount = 0;
			state = Enemy::State::Normal;
		}
		break;
	default:	// それ以外なら何もしない
		break;
	}

	const float enemyHalfW = w * 0.5f;
	const float enemyHalfH = h * 0.5f;

	// エネミーの移動。すでに長さ１なので正規化はいらない
	VECTOR enemyVelocity = VScale(dir, Enemy::Speed);	// 長さ1の向きに、大きさ（速度）をかける
	pos = VAdd(pos, enemyVelocity);				// 座標ベクトルに、velicityを足すことで移動

	// エネミーが画面端からでそうになっていたら画面内の座標に戻してあげ、移動する方向も反転する
	if (pos.x > Graphics::ScreenW - enemyHalfW)
	{
		pos.x = Graphics::ScreenW - enemyHalfW;
		isRightMove = false;
	}
	else if (pos.x < enemyHalfW)
	{
		pos.x = enemyHalfW;
		isRightMove = true;
	}
}

/// <summary>
/// エネミー描画
/// </summary>
void Enemy::Draw() const
{
	const float enemyHalfW = w * 0.5f;
	const float enemyHalfH = h * 0.5f;

	// エネミーを描画
	DrawRotaGraph3(static_cast<int>(pos.x),
		static_cast<int>(pos.y),
		static_cast<int>(enemyHalfW), static_cast<int>(enemyHalfH),
		1.0f, 1.0f,
		0.0f, graph,
		FALSE, FALSE);

#if _DEBUG
	// デバッグ表示:敵の当たり判定の描画
	DrawCircle(static_cast<int>(pos.x),
		static_cast<int>(pos.y),
		static_cast<int>(Enemy::HitSize),
		Debug::EnemyHitSizeColor, 0);
#endif
}

/// <summary>
/// ショット初期化
/// </summary>
void Shot::Initialize()
{
	// --- 全部の弾で共通のデータ
	// ショットのグラフィックをメモリにロード+サイズ取得
	graph = LoadGraph("data/texture/shot.png");

	GetGraphSize(graph, &w, &h);

	// --- 弾の数だけ存在するデータ
	// 弾の位置、ディレクションを作成
	pos = VGet(0, 0, 0);
	dir = VGet(0, -1, 0);	// 弾は常に上にしか移動しない

	// 弾が画面上に存在しているか保持する変数に『存在していない』を意味するfalseを代入しておく
	isAlive = false;
}

/// <summary>
/// ショットの更新
/// </summary>
void Shot::Update(Enemy& enemy)
{
	// 存在状態を保持している変数の内容がtrue(存在する)の場合のみ行う
	if (isAlive == true)
	{
		// 弾を移動させる。dirは常に上方向で長さ１なので、正規化はいらない
		VECTOR shotVelocity = VScale(dir, Shot::Speed);
		pos = VAdd(pos, shotVelocity);

		// 画面外に出てしまった場合は存在状態を保持している変数にfalse(存在しない)を代入する
		if (pos.y < Shot::AliveLimitY)
		{
			isAlive = false;
		}

		// 弾が敵にぶつかっていたら、敵の状態をダメージ状態に
		VECTOR	shotToEnemy = VSub(enemy.pos, pos);	// ショットから敵へのベクトル
		float	shotToEnemyLength = VSize(shotToEnemy);			// ショットから敵への距離
		if (shotToEnemyLength < Enemy::HitSize + Shot::HitSize)
		{
			// 円（または球）同士の当たり判定
			// ショットから敵への距離がお互いの当たり判定サイズより小さい＝当たっている
			enemy.state = Enemy::State::Damage;
			enemy.damageCount = Enemy::DamageTime;

			// 弾も消す
			isAlive = false;
		}
	}
}

/// <summary>
///  ショット描画
/// </summary>
void Shot::Draw() const
{
	if (isAlive == true)
	{
		// 弾を描画する
		const float shotHalfW = w * 0.5f;
		const float shotHalfH = h * 0.5f;
		DrawRotaGraph3(static_cast<int>(pos.x),
			static_cast<int>(pos.y),
			static_cast<int>(shotHalfW), static_cast<int>(shotHalfH),
			1.0f, 1.0f,
			0.0f, graph,
			FALSE, FALSE);

#if _DEBUG
		// デバッグ表示:弾の当たり判定の描画
		DrawCircle(static_cast<int>(pos.x),
			static_cast<int>(pos.y),
			static_cast<int>(Shot::HitSize),
			Debug::ShotHitSizeColor, 0);
#endif
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

	// 各クラスのインスタンス準備
	Input	input;
	Player	player;
	Enemy	enemy;
	std::vector<Shot>	shotArray(Shot::Num);

	// 各クラスの初期化
	//Input::Initialize(input);	// static関数ならこれ
	input.Initialize();	// クラスごとに準備される関数なので、.でアクセス
	player.Initialize();
	enemy.Initialize();

	//for (int i = 0; i < Shot::Num; i++)
	//{
	//	shot[i].Initialize();
	//}
	for (auto& item : shotArray)
	{
		item.Initialize();
	}

	// ゲームループ.
	while (1)
	{
		auto prevTime = GetNowHiPerformanceCount();	// 処理が始まる前の時間

		// 画面を初期化(真っ黒にする)
		ClearDrawScreen();
		
		// 各クラスのUpdate
		input.Update();		// ボタン情報を更新
		player.Update(input, shotArray);
		enemy.Update();
		for (auto& item : shotArray)
		{
			item.Update(enemy);
		}

		// 各クラスのDraw
		player.Draw();
		enemy.Draw();
		for (auto& item : shotArray)
		{
			item.Draw();
		}
		
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