#pragma once

class WorldSprite;

/// <summary>
/// 背景クラス
/// </summary>
class Background
{
public:
	int				modelHandle;
	int				spriteGraph;
	WorldSprite		worldSprite;

	// モデルの表示スケールと位置補正
	// 適当な位置、適当なスケールで雑に背景のモデルを後ろの方に表示する
	inline const static VECTOR	ModelScale	= VGet(0.1f, 0.1f, 0.001f);
	inline const static VECTOR	Pos			= VGet(8.0f, -5.0f, 1.0f);

	void Initialize();
	void Draw() const;
};