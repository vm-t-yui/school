#pragma once

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
		Stand = 0,		// 立ち止まり
		Run = 1,		// 走り
		Jump = 2,		// ジャンプ
	};

	// プレイヤーのアニメーション種別
	enum class AnimKind : int
	{
		None = -1,		// なし
		Unknown = 0,	// 不明
		Run = 1,		// 走り
		Jump = 2,		// ジャンプ
		Fall = 3,		// 落下中
		Stop = 4,		// 立ち止まり
	};

	Player();
	~Player();

	void Load();		// ロード
	void Unload();		// アンロード
	void Update(const Input& input, const Camera& camera, Stage& stage);
	void Draw(const Stage& stage);

	void OnHitRoof();	// 天井に当たった時
	void OnHitFloor();	// 床に当たった時
	void OnFall();		// 落下が確定したとき

	const VECTOR& GetPosition() const { return position; }
	bool GetIsMove() const { return isMove; }
	State GetState() const { return currentState; }
	float GetJumpPower() const { return currentJumpPower; }

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

	VECTOR		position;				// 座標
	VECTOR		targetMoveDirection;	// モデルが向くべき方向のベクトル
	float		angle;					// モデルが向いている方向の角度
	float		currentJumpPower;		// Ｙ軸方向の速度
	int			modelHandle;			// モデルハンドル
	int			shadowHandle;			// 影画像ハンドル
	State		currentState;			// 状態

	int			currentPlayAnim;		// 再生しているアニメーションのアタッチ番号( -1:何もアニメーションがアタッチされていない )
	float		currentAnimCount;		// 再生しているアニメーションの再生時間
	int			prevPlayAnim;			// 前の再生アニメーションのアタッチ番号( -1:何もアニメーションがアタッチされていない )
	float		prevAnimCount;			// 前の再生アニメーションの再生時間
	float		animBlendRate;			// 現在と過去のアニメーションのブレンド率
	bool		isMove;					// そのフレームで動いたかどうか

	// ルートフレームのＺ軸方向の移動パラメータを無効にする
	void DisableRootFrameZMove();

	// パッド入力によって移動パラメータを設定する
	State UpdateMoveParameterWithPad(const Input& input, const Camera& camera, VECTOR& upMoveVec, VECTOR& leftMoveVec, VECTOR& moveVec);
	
	// 移動処理
	void Move(const VECTOR& MoveVector, Stage& stage);

	void UpdateAnimationState(State prevState);	// アニメーションステートの更新
	void UpdateAngle();							// 回転制御
	void PlayAnim(AnimKind playAnim);			// 新たなアニメーションを再生する
	void UpdateAnimation();						// アニメーション処理
	void DrawShadow(const Stage& stage);		// 影を描画
};