// 2024 Takeru Yui All Rights Reserved.
#include "DxLib.h"
#include "DebugDraw.h"
using namespace YuiLib;

// 実体定義
std::vector<DebugDraw::LineInfo> DebugDraw::lineInfo;
std::vector<DebugDraw::CircleInfo> DebugDraw::circleInfo;

/// <summary>
/// クリア
/// </summary>
void DebugDraw::Clear()
{
	lineInfo.clear();
	circleInfo.clear();
}

/// <summary>
/// 描画
/// </summary>
void DebugDraw::Draw()
{
	for (const auto& item : lineInfo)
	{
		DxLib::DrawLineAA(item.start.x,
			item.start.y,
			item.end.x,
			item.end.y,
			item.color);
	}
	for (const auto& item : circleInfo)
	{
		DxLib::DrawCircleAA(item.center.x,
			item.center.y,
			item.radius,
			128,
			item.color,
			FALSE);
	}
}

/// <summary>
/// ライン描画
/// </summary>
void DebugDraw::DrawLine(const VECTOR& start, const VECTOR& end, int color)
{
	LineInfo newInfo;
	newInfo.start = start;
	newInfo.end = end;
	newInfo.color = color;
	lineInfo.emplace_back(newInfo);
}

/// <summary>
/// 円描画
/// </summary>
void DebugDraw::DrawCircle(const VECTOR& center, float radius, int color)
{
	CircleInfo newInfo;
	newInfo.center = center;
	newInfo.radius = radius;
	newInfo.color = color;
	circleInfo.emplace_back(newInfo);
}