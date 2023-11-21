class Input;
class Camera;
class Stage;

/// <summary>
/// プレイヤー
/// </summary>
class Player
{
public:
	// 状態
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
	void Initialize();						// 初期化
	void Finalize();						// 後始末
	void Update(const Input& input, const Camera& camera, Stage& stage);						
	void Draw(const Stage& stage);

	void OnHitRoof();	// 天井に当たった時
	void OnHitFloor();	// 床に当たった時
	void OnFall();		// 落下が確定したとき

	const VECTOR& GetPosition() const { return Position; }
	bool GetIsMove() const { return IsMove; }
	State GetState() const { return State; }
	float GetJumpPower() const { return CurrentJumpPower; }

	// プレイヤー関係の定義
	static constexpr float	FallUpPower = 20.0f;	// 足を踏み外した時のジャンプ力

private:
	// プレイヤー関係の定義
	static constexpr float	PlayAnimSpeed	= 250.0f;	// アニメーション速度
	static constexpr float	MoveSpeed		= 30.0f;	// 移動速度
	static constexpr float	AnimBlendSpeed	= 0.1f;		// アニメーションのブレンド率変化速度
	static constexpr float	AngleSpeed		= 0.2f;		// 角度変化速度
	static constexpr float	JumpPower		= 100.0f;	// ジャンプ力
	static constexpr float	Gravity			= 3.0f;		// 重力
	static constexpr float	ShadowSize		= 200.0f;	// 影の大きさ
	static constexpr float	ShadowHeight	= 700.0f;	// 影が落ちる高さ

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
	bool		IsMove;					// そのフレームで動いたかどうか

	// ルートフレームのＺ軸方向の移動パラメータを無効にする
	void DisableRootFrameZMove();

	// パッド入力によって移動パラメータを設定する
	bool UpdateMoveParameterWithPad(const Input& input, const Camera& camera, VECTOR& UpMoveVec, VECTOR& LeftMoveVec, VECTOR& MoveVec);
	
	// 移動処理
	void Move(const VECTOR& MoveVector, Stage& stage);

	void UpdateAngle();									// 回転制御
	void PlayAnim(AnimKind PlayAnim);					// 新たなアニメーションを再生する
	void UpdateAnimation();								// アニメーション処理
	void DrawShadow(const Stage& stage);				// 影を描画
};