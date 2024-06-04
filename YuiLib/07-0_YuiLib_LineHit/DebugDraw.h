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
	static void Clear();
	static void Draw();

	// ライン描画
	static void DrawLine(const VECTOR& start, const VECTOR& end, int color);

	// サークル描画
	static void DrawCircle(const VECTOR& center, float radius, int color);

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
	};
	static std::vector<LineInfo>	lineInfo;
	static std::vector<CircleInfo>	circleInfo;
};

}