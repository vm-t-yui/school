
// プレイヤー関係の定義
#define PLAYER_PLAY_ANIM_SPEED			250.0f		// アニメーション速度
#define PLAYER_MOVE_SPEED				30.0f		// 移動速度
#define PLAYER_ANIM_BLEND_SPEED			0.1f		// アニメーションのブレンド率変化速度
#define PLAYER_ANGLE_SPEED				0.2f		// 角度変化速度
#define PLAYER_JUMP_POWER				100.0f		// ジャンプ力
#define PLAYER_FALL_UP_POWER			20.0f		// 足を踏み外した時のジャンプ力
#define PLAYER_GRAVITY					3.0f		// 重力
#define PLAYER_MAX_HITCOLL				2048		// 処理するコリジョンポリゴンの最大数
#define PLAYER_ENUM_DEFAULT_SIZE		800.0f		// 周囲のポリゴン検出に使用する球の初期サイズ
#define PLAYER_HIT_WIDTH				200.0f		// 当たり判定カプセルの半径
#define PLAYER_HIT_HEIGHT				700.0f		// 当たり判定カプセルの高さ
#define PLAYER_HIT_TRYNUM				16			// 壁押し出し処理の最大試行回数
#define PLAYER_HIT_SLIDE_LENGTH			5.0f		// 一度の壁押し出し処理でスライドさせる距離
#define PLAYER_SHADOW_SIZE				200.0f		// 影の大きさ
#define PLAYER_SHADOW_HEIGHT			700.0f		// 影が落ちる高さ

class Input;
class Camera;
class Stage;

// 状態
enum class PlayerState : int
{
	STAND = 0,		// 立ち止まり
	RUN = 1,		// 走り
	JUMP = 2,		// ジャンプ
};

// アニメーション種別
enum class PlayerAnimKind : int
{
	NONE = -1,		// なし
	UNKNOWN = 0,	// 不明
	RUN = 1,		// 走り
	JUMP = 2,		// ジャンプ
	FALL = 3,		// 落下中
	STOP = 4,		// 立ち止まり
};

// プレイヤー情報構造体
class Player
{
public:
	void Initialize();						// 初期化
	void Finalize();						// 後始末
	void Update(const Input& input, const Camera& camera, const Stage& stage);						
	void Draw(const Stage& stage);

	const VECTOR& GetPosition() const { return Position; }

private:
	VECTOR		Position;				// 座標
	VECTOR		TargetMoveDirection;	// モデルが向くべき方向のベクトル
	float		Angle;					// モデルが向いている方向の角度
	float		JumpPower;				// Ｙ軸方向の速度
	int			ModelHandle;			// モデルハンドル
	int			ShadowHandle;			// 影画像ハンドル
	PlayerState	State;					// 状態

	int			CurrentPlayAnim;		// 再生しているアニメーションのアタッチ番号( -1:何もアニメーションがアタッチされていない )
	float		CurrentAnimCount;		// 再生しているアニメーションの再生時間
	int			PrevPlayAnim;			// 前の再生アニメーションのアタッチ番号( -1:何もアニメーションがアタッチされていない )
	float		PrevAnimCount;			// 前の再生アニメーションの再生時間
	float		AnimBlendRate;			// 現在と過去のアニメーションのブレンド率

	void Move(VECTOR MoveVector, const Stage& stage);	// 移動処理
	void UpdateAngle();									// 回転制御
	void PlayAnim(PlayerAnimKind PlayAnim);				// 新たなアニメーションを再生する
	void UpdateAnimation();								// アニメーション処理
	void DrawShadow(const Stage& stage);				// 影を描画
};