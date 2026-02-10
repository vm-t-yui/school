#pragma once

class Input;	// プロトタイプ宣言

/// <summary>
/// プレイヤークラス
/// </summary>
class Player final
{
public:
	// 状態を表す列挙体
	enum class State : int
	{
		Normal,		// 通常
		Attack		// 攻撃
	};
	Player();
	virtual ~Player() { /* 処理なし */ }
	void Initialize();
	void Update(const Input& input);
	void Draw() const;

	void ChangeAnimation(int animationIndex, int sourceAnimationModelHandle, float speed, bool isLoop);	// アニメーションの切り替え

private:
	VECTOR	pos;
	VECTOR	hitCenterPos;
	VECTOR	dir;
	State	state;

	int		modelHandle;					// 3Dモデルのハンドル（graphの3D版）
	int		idleAnimationModelHandle;		// 待機アニメーションモデルハンドル
	int		attackAnimationModelHandle;		// 攻撃アニメーションモデルハンドル

	int		currentAnimAttachIndex;			// アタッチしたアニメーションのインデックス
	float	currentAnimTotalTime;			// 再生中のアニメーションのトータル時間
	float	currentAnimTimeCount;			// 再生中のアニメーションの現在時間
	float	currentAnimSpeed;				// 再生中のアニメーションスピード
	bool	currentAnimIsLoop;				// ループするかどうか
	bool	isAnimEnd;						// アニメーションが完了したかどうか
};
