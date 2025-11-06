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
	int		graph;
	int		w, h;

	int		modelHandle;		// 3Dモデルのハンドル（graphの3D版）

	// constexpr定数は後ろにstatic(C++17以降)
	constexpr static float	Speed	= 3.0f;
	constexpr static float	HitSize = 30;	// 当たり判定サイズ

	// constは頭にinline,後ろにstatic(C++17以降)
	inline const static	VECTOR	FirstPos = VGet(Graphics::ScreenW * 0.5f, Graphics::ScreenH - 80.0f, 0);

	// モデルの表示スケールと位置補正
	inline const static VECTOR	ModelScale	= VGet(0.002f, 0.002f, 0.002f);
	inline const static VECTOR	ModelFixPos	= VGet(0.0f, -0.3f, 0.0f);

	void Initialize();
	void Update(const Input& input, std::vector<Shot>& shotArray);
	void Draw() const;
};
