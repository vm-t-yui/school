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

	// constexpr定数は後ろにstatic(C++17以降)
	constexpr static float	Speed = 3.0f;

	// constは頭にinline,後ろにstatic(C++17以降)
	inline const static	VECTOR	FirstPos = VGet(Graphics::ScreenW * 0.5f, Graphics::ScreenH - 80.0f, 0);

	void Initialize();
	void Update(const Input& input, std::vector<Shot>& shotArray);
	void Draw() const;
};
