#pragma once

class Input;	// プロトタイプ宣言
class Shot;		// プロトタイプ宣言

/// <summary>
/// プレイヤークラス
/// </summary>
class Player
{
public:
	VECTOR	pos;
	VECTOR	dir;

	int		modelHandle;					// 3Dモデルのハンドル（graphの3D版）
	int		idleAnimationModelHandle;		// 待機アニメーションモデルハンドル
	int		attackAnimationModelHandle;		// 攻撃アニメーションモデルハンドル

	int		currentAnimAttachIndex;			// アタッチしたアニメーションのインデックス
	float	currentAnimTotalTime;			// 再生中のアニメーションのトータル時間
	float	currentAnimTimeCount;			// 再生中のアニメーションの現在時間
	float	currentAnimSpeed;				// 再生中のアニメーションスピード
	bool	currentAnimIsLoop;				// ループするかどうか

	// constexpr定数は後ろにstatic(C++17以降)
	constexpr static float	Speed	= 8.0f;
	constexpr static float	HitSize = 30;	// 当たり判定サイズ
	
	constexpr static int	IdleAnimationIndex		= 1;	// 待機アニメーションインデックス
	constexpr static float	IdleAnimationSpeed		= 0.3f;	// 待機アニメーションスピード
	constexpr static int	AttackAnimationIndex	= 1;	// 攻撃アニメーションインデックス
	constexpr static float	AttackAnimationSpeed	= 0.8f;	// 攻撃アニメーションスピード

	// constは頭にinline,後ろにstatic(C++17以降)
	inline const static	VECTOR	FirstPos = VGet(100.0f, Graphics::ScreenH * 0.5f, 0);

	// モデルの表示スケールと位置補正
	inline const static VECTOR	ModelScale	= VGet(0.005f, 0.005f, 0.005f);
	inline const static VECTOR	ModelFixPos	= VGet(0.0f, -0.3f, 0.0f);

	void Initialize();
	void Update(const Input& input, std::vector<Shot>& shotArray);
	void Draw() const;

	void ChangeAnimation(int animationIndex, int sourceAnimationModelHandle, float speed, bool isLoop);	// アニメーションの切り替え
};
