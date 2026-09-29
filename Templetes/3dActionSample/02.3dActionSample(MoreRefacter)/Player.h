#pragma once

class Input;
class Camera;
class Stage;

// プレイヤー情報構造体
class Player
{
public:
	// プレイヤーの状態
	enum class State : int
	{
		STAND = 0,		// 立ち止まり
		RUN = 1,		// 走り
		JUMP = 2,		// ジャンプ
	};

	// プレイヤーのアニメーション種別
	enum class AnimKind : int
	{
		NONE = -1,		// なし
		UNKNOWN = 0,	// 不明
		RUN = 1,		// 走り
		JUMP = 2,		// ジャンプ
		FALL = 3,		// 落下中
		STOP = 4,		// 立ち止まり
	};

	// プレイヤー関係の定義
	static const int MaxHitColl;	// 処理するコリジョンポリゴンの最大数

	static constexpr float	PlayAnimSpeed = 250.0f;		// アニメーション速度
	static constexpr float	MoveSpeed = 30.0f;		// 移動速度
	static constexpr float	AnimBlendSpeed = 0.1f;		// アニメーションのブレンド率変化速度
	static constexpr float	AngleSpeed = 0.2f;		// 角度変化速度
	static constexpr float	JumpPower = 100.0f;		// ジャンプ力
	static constexpr float	FallUpPower = 20.0f;		// 足を踏み外した時のジャンプ力
	static constexpr float	Gravity = 3.0f;		// 重力
	static constexpr float	DefaultSize = 800.0f;		// 周囲のポリゴン検出に使用する球の初期サイズ
	static constexpr float	HitWidth = 200.0f;		// 当たり判定カプセルの半径
	static constexpr float	HitHeight = 700.0f;		// 当たり判定カプセルの高さ
	static constexpr int	HitTryNum = 16;		// 壁押し出し処理の最大試行回数
	static constexpr float	HitSlideLength = 5.0f;		// 一度の壁押し出し処理でスライドさせる距離
	static constexpr float	ShadowSize = 200.0f;		// 影の大きさ
	static constexpr float	ShadowHeight = 700.0f;		// 影が落ちる高さ

	void Initialize();						// 初期化
	void Finalize();						// 後始末
	void Update(const Input& input, const Camera& camera, const Stage& stage);						
	void Draw(const Stage& stage);

	const VECTOR& GetPosition() const { return Position; }

private:
	VECTOR		Position;				// 座標
	VECTOR		TargetMoveDirection;	// モデルが向くべき方向のベクトル
	float		Angle;					// モデルが向いている方向の角度
	float		CurrentJumpPower;		// Ｙ軸方向の速度
	int			ModelHandle;			// モデルハンドル
	int			ShadowHandle;			// 影画像ハンドル
	State		State;					// 状態

	int			CurrentPlayAnim;		// 再生しているアニメーションのアタッチ番号( -1:何もアニメーションがアタッチされていない )
	float		CurrentAnimCount;		// 再生しているアニメーションの再生時間
	int			PrevPlayAnim;			// 前の再生アニメーションのアタッチ番号( -1:何もアニメーションがアタッチされていない )
	float		PrevAnimCount;			// 前の再生アニメーションの再生時間
	float		AnimBlendRate;			// 現在と過去のアニメーションのブレンド率


	// ルートフレームのＺ軸方向の移動パラメータを無効にする
	void DisableRootFrameZMove();

	// パッド入力によって移動パラメータを設定する
	bool UpdateMoveParameterWithPad(const Input& input, const Camera& camera, VECTOR& UpMoveVec, VECTOR& LeftMoveVec, VECTOR& MoveVec);
	
	// 移動処理
	void Move(const VECTOR& MoveVector, const Stage& stage);

	// 検出されたポリゴンが壁ポリゴン( ＸＺ平面に垂直なポリゴン )か床ポリゴン( ＸＺ平面に垂直ではないポリゴン )かを判断し、保存する
	void CheckKabeAndYuka(MV1_COLL_RESULT_POLY** Kabe, MV1_COLL_RESULT_POLY** Yuka, int& KabeNum, int& YukaNum, MV1_COLL_RESULT_POLY_DIM HitDim);

	// 壁ポリゴンとの当たりをチェックし、移動ベクトルを補正する
	void FixNowPositionWithKabe(VECTOR& NowPos, const VECTOR& OldPos, const VECTOR& MoveVector, bool IsMove,  MV1_COLL_RESULT_POLY** Kabe, int KabeNum);
	void FixNowPositionWithKabeInternal(VECTOR& NowPos, MV1_COLL_RESULT_POLY** Kabe, int KabeNum);

	// 床ポリゴンとの当たりをチェックし、移動ベクトルを補正する
	void FixNowPositionWithYuka(VECTOR& NowPos, bool IsMove, MV1_COLL_RESULT_POLY** Yuka, int YukaNum);

	void UpdateAngle();									// 回転制御
	void PlayAnim(AnimKind PlayAnim);					// 新たなアニメーションを再生する
	void UpdateAnimation();								// アニメーション処理
	void DrawShadow(const Stage& stage);				// 影を描画
};