// 2024 Takeru Yui All Rights Reserved.
#pragma once

#include <vector>

namespace YuiLib {

/// <summary>
/// デバッグ用の描画情報をまとめ、後で表示するためのクラス
/// </summary>
class DebugDraw
{
public:
	static void Initialize();
	static void Finalize();
	static void Clear();
	static void Draw3D();

	// ライン描画
	static void DrawLine(const VECTOR& start, const VECTOR& end, int color);

	// サークル描画
	static void DrawCircle(const VECTOR& center, float radius, int color, float alpha = 1.0f, bool isWire=true);

private:
	// ライン情報
	struct LineInfo
	{
		VECTOR	start;
		VECTOR	end;
		int		color;
	};
	// サークル情報
	struct CircleInfo
	{
		VECTOR	center;
		float	radius;
		int		color;
		float	alpha;
		bool	isWire;
	};
	enum class HandleKind : int
	{
		Circle = 0,
		CircleWire,
		Square,
		SquareWire,
		Num
	};
	static std::vector<LineInfo>		lineInfo;
	static std::vector<CircleInfo>		circleInfo;
	static std::vector<int>				handles;
	static std::vector<WorldSprite*>	spites;
};

}